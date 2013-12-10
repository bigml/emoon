#include "mheads.h"
#include "lheads.h"

NEOERR* lrend_init()
{
    NEOERR *err;

    err = lentity_init();
    if (err != STATUS_OK) return nerr_pass(err);

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

NEOERR* lrend_update(float dt)
{
    return STATUS_OK;
}

NEOERR* lrend_shadowmap_rend()
{
    RendEntity *e;
    char *key;
    HASH *eh;

    eh = hash_lookup(g_datah, ENTITY_KEY);

    MCS_NOT_NULLA(eh);

    key = NULL;
    e = hash_next(eh, (void**)&key);
    while (e) {
        if (e->typeid == ENTITY_TYPE_CAR) mrend_shadowmap_rend_static(e);

        e = hash_next(eh, (void**)&key);
    }

    return STATUS_OK;
}

NEOERR* lrend_forwardrend_rend()
{
    RendEntity *e;
    char *key;
    HASH *eh;

    eh = hash_lookup(g_datah, ENTITY_KEY);

    MCS_NOT_NULLA(eh);

    key = NULL;
    e = hash_next(eh, (void**)&key);
    while (e) {
        if (e->typeid == ENTITY_TYPE_CAR) mrend_forwardrend_rend_static(e);

        e = hash_next(eh, (void**)&key);
    }

    return STATUS_OK;
}

void lrend_finish()
{
    masset_finish();
    mentity_finish();
}
