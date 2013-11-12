#ifndef __MCAMERA_H__
#define __MCAMERA_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    RendEntity base;

    vec3 target;
    float fov;
    float near_clip;
    float far_clip;
} CameraEntity;

EntityDriver camera_entity_driver;

mat4 mentity_camera_view_matrix(CameraEntity *c);
mat4 mentity_camera_proj_matrix(CameraEntity *c, float aspect_ratio);

__END_DECLS
#endif    /* __MCAMERA_H__ */
