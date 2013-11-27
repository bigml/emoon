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
    MAT_TYPE_TEXTURE,
    MAT_TYPE_TEXTURE_3D
};

NEOERR* mast_mat_add_item_int(MatEntry *me, char *name, int val);
NEOERR* mast_mat_add_item_float(MatEntry *me, char *name, float val);
NEOERR* mast_mat_add_item_vec2(MatEntry *me, char *name, vec2 val);
NEOERR* mast_mat_add_item_vec3(MatEntry *me, char *name, vec3 val);
NEOERR* mast_mat_add_item_vec4(MatEntry *me, char *name, vec4 val);
NEOERR* mast_mat_add_item_shader(MatEntry *me, char *name, ShaderAsset *val);
NEOERR* mast_mat_add_item_texture(MatEntry *me, char *name, TexAsset *val);
NEOERR* mast_mat_add_item_texture3d(MatEntry *me, char *name, TexAsset *val);
NEOERR* mast_mat_add_item_asset(MatEntry *me, char *name, int type, RendAsset *val);

NEOERR* mast_mat_load(char *dir, char *name, RendAsset **a);
void    mast_mat_unload(void *p);

__END_DECLS
#endif  /* __MASSETMAT_H__ */
