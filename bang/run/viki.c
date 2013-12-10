/*
 * viki 主脑
 */

#include "mheads.h"
#include "lheads.h"

HDF  *g_cfg = NULL;
HASH *g_datah = NULL;
uint32_t g_time_upms = 0;

int main(int argc, char **argv)
{
    uint32_t time_lastms;
    float dt;
    NEOERR *err;

    msys_makesure_coredump();

    err = mcfg_parse_file(APPCONFIG, &g_cfg);
    OUTPUT_NOK(err);

    err = lerr_init();
    DIE_NOK(err);

    mtc_init(hdf_get_value(g_cfg, PRE_CONFIG".trace_file", "viki"),
             hdf_get_int_value(g_cfg, PRE_CONFIG".trace_level", TC_DEFAULT_LEVEL));

    err = mrend_init(hdf_get_value(g_cfg, "Window.title", "viki"),
                     hdf_get_int_value(g_cfg, "Window.x", 0),
                     hdf_get_int_value(g_cfg, "Window.y", 0),
                     hdf_get_int_value(g_cfg, "Window.w", 100),
                     hdf_get_int_value(g_cfg, "Window.h", 100),
                     hdf_get_int_value(g_cfg, "Window.flags", 1));
    DIE_NOK(err);

    err = lrend_init();
    DIE_NOK(err);

    err = mact_init();
    DIE_NOK(err);

    err = lact_init();
    DIE_NOK(err);

    g_time_upms = 0;
    time_lastms = 0;

    bool running = true;
    SDL_Event e;
    while (running) {
        dt = ((float)g_time_upms - time_lastms) / 1000.0;
        time_lastms = g_time_upms;

        while (SDL_PollEvent(&e)) {
            mact_on(e, &running);
            lact_on(e, &running);
        }

        mrend_update(dt);
        lrend_update(dt);

        mrend_shadowmap_begin();
        mrend_shadowmap_rend();
        lrend_shadowmap_rend();
        mrend_shadowmap_end();

        mrend_forwardrend_begin();
        mrend_forwardrend_rend();
        lrend_forwardrend_rend();
        mrend_forwardrend_end();

        mrend_present();
    }

    lact_finish();
    lrend_finish();
    mact_finish();
    mrend_finish();
    mcfg_cleanup(&g_cfg);

    return 0;
}
