#ifndef __MGL_H__
#define __MGL_H__

#include "mheads.h"

__BEGIN_DECLS

void mgl_push_matrix();
void mgl_rend_texture(GLuint tex);
void mgl_pop_matrix();

__END_DECLS

#endif  /* __MGL_H__ */
