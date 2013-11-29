#include "mheads.h"
#include "lheads.h"

NEOERR* lrend_init()
{
    NEOERR *err;

    err = mentity_load_dir(PATH_TPL"entity/", PATH_FLY);
    if (err != STATUS_OK) return nerr_pass(err);

    err = mrend_shadowmap_init(PATH_FLY"base/", (LightEntity*)mentity_node_get("sun"));
    if (err != STATUS_OK) return nerr_pass(err);

    err = mrend_forwardrend_init(PATH_FLY"base/");
    if (err != STATUS_OK) return nerr_pass(err);

    mrend_forwardrend_set_camera((CameraEntity*)mentity_node_get("camera"));
    mrend_forwardrend_set_shadow_light((LightEntity*)mentity_node_get("sun"));
    mrend_forwardrend_set_shadow_texture(mrend_shadowmap_depth_texture());
    mrend_forwardrend_add_light((LightEntity*)mentity_node_get("sun"));
    mrend_forwardrend_add_light((LightEntity*)mentity_node_get("backlight"));

    return STATUS_OK;
}

NEOERR* lrend_update()
{
    return STATUS_OK;
}

NEOERR* lrend_rend()
{
    return STATUS_OK;
}

void lrend_finish()
{
    masset_finish();
    mentity_finish();
}
