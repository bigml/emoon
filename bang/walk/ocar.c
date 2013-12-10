#include "mheads.h"
#include "ocar.h"

static void lentity_car_free(void *p)
{
    DynamicEntity *e = p;

    if (!e) return;

    SAFE_FREE(e->me);

    mentity_dynamic_free(p);
}

static NEOERR* lentity_car_new(HDF *enode, char *dir, RendEntity **pe)
{
    NEOERR *err;

    err = mentity_dynamic_new(enode, dir, pe);
    if (err != STATUS_OK) return nerr_pass(err);

    (*pe)->typeid = car_entity_driver.typeid;

    DynamicEntity* e = (DynamicEntity*)(*pe);

    e->me = calloc(1, sizeof(CarEntity));
    if (!e->me) return nerr_raise(NERR_NOMEM, "car entity");

    return STATUS_OK;
}

static NEOERR* lentity_car_onact(RendEntity *pent, SDL_Event e, bool *running)
{
    DynamicEntity *ent = (DynamicEntity*)pent;
    CarEntity *me = (CarEntity*)ent->me;

    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
            me->radius += 0.06;
            ent->rotation = quaternion_yaw(me->radius);
            break;
        case SDLK_RIGHT:
            me->radius -= 0.06;
            ent->rotation = quaternion_yaw(me->radius);
            break;
        case SDLK_UP:
            ent->act.fa_on = true;
            break;
        case SDLK_DOWN:
            break;
        default:
            break;
        }
    }

    return STATUS_OK;
}

static NEOERR* lentity_car_update(RendEntity *p, float dt)
{
    DynamicEntity *e = (DynamicEntity*)p;
    CarEntity *me = (CarEntity*)e->me;

    if (!vec3_equ(e->phy.v, vec3_zero())) e->act.fb_on = true;

    //e->phy.f = vec3_add(e->evt.f, vec3_new(0, 0, e->phy.ffric));
    //vec3 v = mat3_mul_vec3(mat3_rotation_y(e->phy.radius), e->phy.v);
    //vec3 a = mat3_mul_vec3(mat3_rotation_y(e->phy.radius), e->phy.a);

    mentity_dynamic_update(p, dt);

    return STATUS_OK;
}

EntityDriver car_entity_driver = {
    .typename = "car",
    .new = lentity_car_new,
    .onact = lentity_car_onact,
    .update = lentity_car_update,
    .free = lentity_car_free,
};
