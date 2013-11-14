#ifndef __MGL_H__
#define __MGL_H__

#include "mheads.h"

__BEGIN_DECLS

void mgl_push_matrix();
void mgl_rend_texture(GLuint tex);
void mgl_pop_matrix();

char* mgl_error_str(GLenum error);
char* mgl_error_str_framebuffer(GLenum error);
char* mgl_format_str(GLenum format);

char* mgl_get_texture_parameters(GLuint objid);
char* mgl_get_renderbuffer_parameters(GLuint objid);

__END_DECLS

#endif  /* __MGL_H__ */
