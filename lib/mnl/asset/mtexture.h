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

__END_DECLS
#endif  /* MASSETTEXTURE_H__ */
