/*
 * viki 主脑
 */

#include "mheads.h"
#include "lheads.h"

HDF  *g_cfg = NULL;
HASH *g_datah = NULL;

int main(int argc, char **argv)
{
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

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            mevt_on(e, &running);
            levt_on(e, &running);
        }

        mrend_update();
        lrend_update();

        mrend_rend();
        lrend_rend();

        mrend_present();
    }

    lrend_finish();
    mrend_finish();
    mcfg_cleanup(&g_cfg);

    return 0;
}
