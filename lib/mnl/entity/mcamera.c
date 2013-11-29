#include "mheads.h"

#define DEFAULT_FOV       0.785398163
#define DEFAULT_NEAR_CLIP 0.1
#define DEFAULT_FAR_CLIP  8192.0

static void mcamera_free(void *p)
{
    CameraEntity *e = p;

    if (!e) return;

    free(e->base.name);
    free(e);
}

static NEOERR* mcamera_new(HDF *enode, char *dir, RendEntity **pe)
{
    char *ename, *etype, *pos, *target;

    MCS_NOT_NULLB(enode, pe);

    *pe = NULL;

    ename = hdf_obj_name(enode);
    etype = mcs_obj_attr(enode, "type");
    pos = hdf_get_value(enode, "position", "0 0 0");
    target = hdf_get_value(enode, "target", "0 0 0");

    if (etype && strcmp(etype, "camera"))
        return nerr_raise(NERR_ASSERT, "type %s not match", etype);

    CameraEntity *e = calloc(1, sizeof(CameraEntity));
    if (!e) return nerr_raise(NERR_NOMEM, "alloc entity");

    e->base.name = strdup(ename);
    e->base.typeid = ENTITY_TYPE_CAMERA;
    e->base.active = true;
    e->base.receive_shadows = true;
    e->base.cast_shadows = true;
    e->base.position = vec3_from_string(pos);

    /* s must be a float macro */
#define str(s) #s
#define xstr(s) str(s)
#define SET_CONFIG_1F(val, key, def) val = atof(hdf_get_value(enode, key, xstr(def)))

    e->target = vec3_from_string(target);
    SET_CONFIG_1F(e->fov, "fov", DEFAULT_FOV);
    SET_CONFIG_1F(e->near_clip, "near_clip", DEFAULT_NEAR_CLIP);
    SET_CONFIG_1F(e->far_clip,  "far_clip",  DEFAULT_FAR_CLIP);

    *pe = (RendEntity*)e;

    return STATUS_OK;
}

EntityDriver camera_entity_driver = {
    .name = "camera",
    .new = mcamera_new,
    .free = mcamera_free,
};


mat4 mentity_camera_view_matrix(CameraEntity *c)
{
    return mat4_view_look_at(c->base.position, c->target, vec3_new(0.0f, 1.0f, 0.0f));
}

mat4 mentity_camera_proj_matrix(CameraEntity *c, float aspect_ratio)
{
    return mat4_perspective(c->fov, c->near_clip, c->far_clip, aspect_ratio);
}

void mentity_camera_control_orbit(CameraEntity *c, SDL_Event e)
{
    float a1 = 0;
    float a2 = 0;
    int x, y, dx, dy;
    vec3 axis;
    Uint8 state;

    vec3 translation = c->target;
    c->base.position = vec3_sub(c->base.position, translation);
    c->target = vec3_sub(c->target, translation);

    state = SDL_GetMouseState(&x, &y);

    switch (e.type) {
    case SDL_MOUSEMOTION:
        state = SDL_GetMouseState(&x, &y);
        SDL_GetRelativeMouseState(&dx, &dy);
        if (state & SDL_BUTTON_LMASK) {
            a1 = dx * -0.005;
            a2 = dy * 0.005;
            c->base.position = mat3_mul_vec3(mat3_rotation_y( a1 ), c->base.position );
            axis = vec3_normalize(vec3_cross(vec3_sub(c->base.position, c->target),
                                             vec3_new(0,1,0)));
            c->base.position = mat3_mul_vec3(mat3_rotation_axis_angle(axis, a2),
                                             c->base.position);
        }
        break;
    case SDL_MOUSEWHEEL:
        if (e.wheel.y > 0) {
            c->base.position = vec3_sub(c->base.position,
                                        vec3_normalize(c->base.position));
        }
        if (e.wheel.y < 0) {
            c->base.position = vec3_add(c->base.position,
                                        vec3_normalize(c->base.position));
        }
        break;
    }

    c->base.position = vec3_add(c->base.position, translation);
    c->target = vec3_add(c->target, translation);
}
