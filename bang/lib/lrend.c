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
    StaticEntity *ent = (StaticEntity*)mentity_node_get("focus");
    if (ent) {
        if (!ent->evt.engine_on && abs(g_time_upms - ent->evt.engine_hb) > 1000) {
            ent->evt.f = vec3_zero();
            if (vec3_length(ent->phy.v) < 1.0) {
                ent->phy.v = vec3_zero();
            }
        }

        if (!vec3_equ(ent->phy.v, vec3_zero()))
            ent->phy.f = vec3_add(ent->evt.f, vec3_new(0, 0, ent->phy.ffric));
        else ent->phy.f = ent->evt.f;

        if (!vec3_equ(ent->phy.f, vec3_zero())) {
            ent->phy.a = vec3_div(ent->phy.f, ent->phy.m);
        } else {
            ent->phy.a = vec3_zero();
        }

        vec3 v = mat3_mul_vec3(mat3_rotation_y(ent->phy.radius), ent->phy.v);
        vec3 a = mat3_mul_vec3(mat3_rotation_y(ent->phy.radius), ent->phy.a);

        float t = ((float)g_time_upms - ent->t) / 1000.0;
        vec3 at2 = vec3_mul(a, pow(t, 2.0));
        vec3 v0t = vec3_mul(v, t);
        vec3 s = vec3_add(v0t, vec3_div(at2, 2));

        ent->t = g_time_upms;
        ent->phy.v = vec3_add(ent->phy.v, vec3_mul(ent->phy.a, t));
        ent->base.position = vec3_add(ent->base.position, s);

        ent->evt.engine_on = false;
    }

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
