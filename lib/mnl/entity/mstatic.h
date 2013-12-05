#ifndef __MSTATIC_H__
#define __MSTATIC_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    RendEntity base;

    vec3 scale;
    vec4 rotation;

    uint32_t t;                   /* t0 */

    struct {
        float radius;           /* rotation radius */
        float m;                /* weight */
        vec3  f;                /* outer force */
        vec3  v;                /* velocity */
        vec3  a;                /* accleration */
        float fpull;            /* engine pull force */
        float ffric;            /* friction force */
        vec3  fmax;             /* max outer force */
    } phy;                      /* PHYSICAL STATU */

    struct {
        bool engine_on;
        uint32_t engine_hb;     /* engine last on time */
        vec3 f;                 /* event force */
    } evt;                      /* EVENT STATU */
} StaticEntity;

EntityDriver static_entity_driver;

__END_DECLS
#endif    /* __MSTATIC_H__ */
