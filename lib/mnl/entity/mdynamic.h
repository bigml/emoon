#ifndef __MDYNAMIC_H__
#define __MDYNAMIC_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    RendEntity base;

    vec3 scale;
    vec4 rotation;

    struct {
        float m;                /* weight */
        vec3  g;                /* gravity */
        vec3  v;                /* velocity */
        vec3  a;                /* accleration */

        vec3  f;                /* outer force f = fa + fb + fc + [fg]*/
        vec3  fa;
        vec3  fb;
        vec3  fc;
    } phy;                      /* PHYSICAL STATU */

    struct {
        bool fa_on;
        bool fb_on;
        bool fc_on;
    } act;                      /* ACTION STATU */

    void *me;
} DynamicEntity;

NEOERR* mentity_dynamic_new(HDF *enode, char *dir, RendEntity **pe);
void    mentity_dynamic_free(void *p);

EntityDriver dynamic_entity_driver;

__END_DECLS
#endif    /* __MDYNAMIC_H__ */
