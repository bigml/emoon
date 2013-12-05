#include "mheads.h"
#include "lheads.h"

NEOERR* lact_init()
{
    return STATUS_OK;
}

NEOERR* lact_on(SDL_Event e, bool *running)
{
    mentity_camera_control_orbit((CameraEntity*)mentity_node_get("camera"), e);

    StaticEntity *ent = (StaticEntity*)mentity_node_get("focus");
    if (!ent) return STATUS_OK;

    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
            ent->phy.radius += 0.06;
            ent->rotation = quaternion_yaw(ent->phy.radius);
            break;
        case SDLK_RIGHT:
            ent->phy.radius -= 0.06;
            ent->rotation = quaternion_yaw(ent->phy.radius);
            break;
        case SDLK_UP:
            ent->evt.engine_on = true;
            if (abs(g_time_upms - ent->evt.engine_hb) > 500) {
                ent->evt.engine_hb = g_time_upms;
                ent->evt.f = vec3_add(ent->evt.f, vec3_new(0, 0, ent->phy.fpull));
                ent->evt.f = vec3_min_vec3_abs(ent->evt.f, ent->phy.fmax);
            }
            break;
        case SDLK_DOWN:
            break;
        default:
            break;
        }
    }

    return STATUS_OK;
}

void lact_finish()
{
    ;
}
