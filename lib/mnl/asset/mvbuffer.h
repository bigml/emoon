#ifndef __MVBUFFER_H__
#define __MVBUFFER_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    GLuint vertex_vbo;
    GLuint triangle_vbo;
    int num_verticies;
    int num_triangles;
} VbufferSurface;

typedef struct {
    RendAsset base;

    VbufferSurface **surfaces;
    int num_surfaces;
} VbufferAsset;

NEOERR* mvb_obj_load(char *dir, char *name, RendAsset **a);
void    mvb_unload(void *p);

__END_DECLS
#endif    /* __MVBUFFER_H__ */
