#include "mheads.h"

#define DEFAULT_FOV       0.785398163
#define DEFAULT_NEAR_CLIP 0.1
#define DEFAULT_FAR_CLIP  8192.0

static int mlight_type(char *type)
{
    if (!type) return -1;

    if (!strcmp(type, "point")) return 0;
    if (!strcmp(type, "directional")) return 1;
    if (!strcmp(type, "sun")) return 2;
    if (!strcmp(type, "spot")) return 3;

    return -1;
}

static void mlight_free(void *p)
{
    LightEntity *e = p;

    if (!e) return;

    free(e->base.name);
    free(e);
}

static NEOERR* mlight_new(HDF *enode, char *dir, RendEntity **pe)
{
    char *ename, *etype, *ltype, *tmps;
    int type;

    MCS_NOT_NULLB(enode, pe);

    *pe = NULL;

    ename = hdf_obj_name(enode);
    etype = mcs_obj_attr(enode, "type");
    ltype = hdf_get_value(enode, "type", "point");

    if (etype && strcmp(etype, "light"))
        return nerr_raise(NERR_ASSERT, "type %s not match", etype);

    LightEntity *e = calloc(1, sizeof(LightEntity));
    if (!e) return nerr_raise(NERR_NOMEM, "alloc entity");

    /*
     * set default value formal, config will overwrite them
     */
    type = mlight_type(ltype);
    if (type < 0) return nerr_raise(NERR_ASSERT, "unknown type %s", ltype);
    mentity_light_set_type(e, type);

    e->base.name = strdup(ename);
    e->base.typeid = ENTITY_TYPE_LIGHT;
    e->base.active = true;
    e->base.receive_shadows = true;
    e->base.cast_shadows = true;
    e->base.position = vec3_from_string(hdf_get_value(enode, "position", "0 0 0"));
    e->base.shadowmat = NULL;

#define SET_CONFIG_1I(val, key)                 \
    tmps = hdf_get_value(enode, key, NULL);     \
    if (tmps) val = atoi(tmps);
#define SET_CONFIG_1F(val, key)                 \
    tmps = hdf_get_value(enode, key, NULL);     \
    if (tmps) val = atof(tmps);
#define SET_CONFIG_3F(val, key)                 \
    tmps = hdf_get_value(enode, key, NULL);     \
    if (tmps) val = vec3_from_string(tmps);

    SET_CONFIG_1F(e->power,    "power");
    SET_CONFIG_1F(e->falloff,  "falloff");
    SET_CONFIG_3F(e->target,   "target");
    SET_CONFIG_3F(e->diffuse,  "diffuse");
    SET_CONFIG_3F(e->specular, "specular");
    SET_CONFIG_3F(e->ambient,  "ambient");
    SET_CONFIG_1I(e->shadow_map_width, "shadow_map_width");
    SET_CONFIG_1I(e->shadow_map_height, "shadow_map_height");
    SET_CONFIG_3F(e->shadow_color, "shadow_color");

    *pe = (RendEntity*)e;

    return STATUS_OK;
}

EntityDriver light_entity_driver = {
    .typename = "light",
    .new = mlight_new,
    .onact = NULL,
    .update = NULL,
    .free = mlight_free,
};


void mentity_light_set_type(LightEntity *e, int type)
{
    e->type = type;

    if(type == LIGHT_TYPE_DIRECTIONAL) {
        e->power = 1;
        e->falloff = 0;

        e->enabled = true;
        e->cast_shadows = false;

        e->orthographic = true;
        e->ortho_width = -1;
        e->ortho_height = -1;

        e->fov = -1;
        e->aspect_ratio = -1;

        e->shadow_color = vec3_zero();
        e->shadow_map_width = -1;
        e->shadow_map_height = -1;
    } else if (type == LIGHT_TYPE_POINT) {
        e->power = 5;
        e->falloff = 0.5;

        e->enabled = true;
        e->cast_shadows = false;

        e->orthographic = false;
        e->ortho_width = -1;
        e->ortho_height = -1;

        e->fov = -1;
        e->aspect_ratio = -1;

        e->shadow_color = vec3_zero();
        e->shadow_map_width = -1;
        e->shadow_map_height = -1;
    } else if (type == LIGHT_TYPE_SUN) {
        e->power = 1.5;
        e->falloff = 0;

        e->enabled = true;
        e->cast_shadows = true;

        e->orthographic = true;
        e->ortho_width = 128;
        e->ortho_height = 128;

        e->fov = -1;
        e->aspect_ratio = -1;

        e->shadow_color = vec3_zero();
        e->shadow_map_width = 2048;
        e->shadow_map_height = 2048;
    } else if (type == LIGHT_TYPE_SPOT) {
        e->power = 5;
        e->falloff = 0.5;

        e->enabled = true;
        e->cast_shadows = true;

        e->orthographic = false;
        e->ortho_width = -1;
        e->ortho_height = -1;

        e->fov = DEFAULT_FOV;
        e->aspect_ratio = 1.0;

        e->shadow_color = vec3_zero();
        e->shadow_map_width = 1024;
        e->shadow_map_height = 1024;
    }
}

vec3 mentity_light_direction(LightEntity *e)
{
    return vec3_normalize(vec3_sub(e->target, e->base.position));
}

mat4 mentity_light_view_matrix(LightEntity *e)
{
    return mat4_view_look_at(e->base.position, e->target, vec3_new(0.0f, 1.0f, 0.0f));
}

mat4 mentity_light_proj_matrix(LightEntity *e)
{
    if (e->orthographic)
        return mat4_orthographic(-e->ortho_width, e->ortho_width,
                                 -e->ortho_height, e->ortho_height,
                                 DEFAULT_NEAR_CLIP, DEFAULT_FAR_CLIP);
    else
        return mat4_perspective(e->fov, DEFAULT_NEAR_CLIP, DEFAULT_FAR_CLIP,
                                e->aspect_ratio);
}
