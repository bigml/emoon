#ifndef __MSTATIC_H__
#define __MSTATIC_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    RendEntity base;

    vec3 scale;
    vec4 rotation;
} StaticEntity;

EntityDriver static_entity_driver;

__END_DECLS
#endif    /* __MSTATIC_H__ */
