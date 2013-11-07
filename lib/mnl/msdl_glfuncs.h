SDL_PROC(void, glGenFramebuffers, (GLsizei n, GLuint* ids))
SDL_PROC(void, glBindFramebuffer, (GLenum target, GLuint framebuffer))

SDL_PROC(void, glGenRenderbuffers, (GLsizei n, GLuint* buffers))
SDL_PROC(void, glBindRenderbuffer, (GLenum target, GLuint buffer))
SDL_PROC(void, glRenderbufferStorage, (GLenum target, GLenum format, GLsizei width, GLsizei height))

SDL_PROC(void, glFramebufferRenderbuffer, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer))
SDL_PROC(void, glFramebufferTexture2D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level))
SDL_PROC(GLenum, glCheckFramebufferStatus, (GLenum target))
SDL_PROC(void, glDeleteFramebuffers, (GLsizei n, GLuint *framebuffers))
SDL_PROC(void, glDeleteRenderbuffers, (GLsizei n, GLuint *buffers))
//SDL_PROC(, gl, ())
