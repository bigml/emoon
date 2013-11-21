#ifndef __MLIGHT_H__
#define __MLIGHT_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    RendEntity base;

    vec3 target;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;

    float power;
    float falloff;

    bool enabled;
    bool cast_shadows;

    int type;

    /* Shadow Mapping */
    vec3 shadow_color;
    int shadow_map_width;
    int shadow_map_height;

    /* Orthographic Shadow Mapping */
    bool orthographic;
    float ortho_width;
    float ortho_height;

    /* Projection Shadow Mapping */
    float fov;
    float aspect_ratio;
} LightEntity;

enum {
    LIGHT_TYPE_POINT = 0,
    LIGHT_TYPE_DIRECTIONAL,
    LIGHT_TYPE_SUN,
    LIGHT_TYPE_SPOT
};

EntityDriver light_entity_driver;

void mentity_light_set_type(LightEntity *e, int type);
vec3 mentity_light_direction(LightEntity *e);

mat4 mentity_light_view_matrix(LightEntity *e);
mat4 mentity_light_proj_matrix(LightEntity *e);

__END_DECLS
#endif    /* __MLIGHT_H__ */
