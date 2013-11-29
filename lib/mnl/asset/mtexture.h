#ifndef __MASSETTEXTURE_H__
#define __MASSETTEXTURE_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    RendAsset base;

    GLuint tex;
} TexAsset;

NEOERR* mast_dds_load(char *dir, char *name, RendAsset **a);
void    mast_dds_unload(void *p);

NEOERR* mast_lut_load(char *dir, char *name, RendAsset **a);
void    mast_lut_unload(void *p);

NEOERR* mast_tex_sdl_load(char *dir, char *name, RendAsset **a);
void    mast_tex_sdl_unload(void *p);

/*
 * It's not load and unload called by masset.c, don't care about base infomation.
 * Caller's response to free it
 */
TexAsset* mast_texture_new(GLuint texid);
NEOERR*   mast_texture_load_raw(char *fname, GLuint *texid);

__END_DECLS
#endif  /* MASSETTEXTURE_H__ */
