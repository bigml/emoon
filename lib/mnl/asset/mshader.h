#ifndef __MASSETSHADER_H__
#define __MASSETSHADER_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    RendAsset base;

    GLuint shader;
} ShaderAsset;

GLuint  mast_shader_prog_new();
NEOERR* mast_shader_prog_attach_shader(GLuint prog, RendAsset *a);
NEOERR* mast_shader_prog_link(GLuint prog);
void    mast_shader_prog_free(GLuint prog);

NEOERR* mast_shader_load(char *dir, char *name, GLenum type, RendAsset **a);
NEOERR* mast_vs_load(char *dir, char *name, RendAsset **a);
NEOERR* mast_fs_load(char *dir, char *name, RendAsset **a);
void    mast_shader_unload(void *p);

__END_DECLS
#endif  /* MASSETSHADER_H__ */
