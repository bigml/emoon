#ifndef __MASSETMAT_H__
#define __MASSETMAT_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    char *name;
    int type;
    union {
        int i;
        float f;
        vec2 v2;
        vec3 v3;
        vec4 v4;
        RendAsset *a;
    } as;
} MatItem;

typedef struct {
    GLuint prog;
    
    ULIST *items;               /* matItem list */
} MatEntry;

typedef struct {
    RendAsset base;

    ULIST *entries;             /* MatEntry list */
} MatAsset;

enum {
    MAT_TYPE_INT = 0,
    MAT_TYPE_FLOAT,
    MAT_TYPE_VEC2,
    MAT_TYPE_VEC3,
    MAT_TYPE_VEC4,
    MAT_TYPE_SHADER,
    MAT_TYPE_TEXTURE
};

NEOERR* mast_mat_load(char *dir, char *name, RendAsset **a);
void    mast_mat_unload(void *p);

__END_DECLS
#endif  /* __MASSETMAT_H__ */
