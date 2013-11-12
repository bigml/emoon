#include "mheads.h"

void mgl_push_matrix()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void mgl_rend_texture(GLuint tex)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, -1.0,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, -1.0,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0,  1.0,  0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0,  1.0,  0.0f);
    glEnd();
    
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
}

void mgl_pop_matrix()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


char* mgl_errstr(GLenum error)
{
    switch (error) {
    case GL_INVALID_ENUM:
        return "gl_error_string_invalid_enum";
    case GL_INVALID_VALUE:
        return "invalid_value";
    case GL_INVALID_OPERATION:
        return "invalid_operation";
    case GL_OUT_OF_MEMORY:
        return "out_of_memory";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "invalid_framebuffer_operation";
    case GL_STACK_OVERFLOW:
        return "stack_overflow";
    case GL_STACK_UNDERFLOW:
        return "stack_underflow";
    case GL_TABLE_TOO_LARGE:
        return "table_too_large";
    }
    return "no_error";
}

char* mgl_errstr_framebuffer(GLenum error)
{
    switch(error) {
    case GL_FRAMEBUFFER_COMPLETE:
        return "framebuffer_complete";
    case GL_FRAMEBUFFER_UNDEFINED:
        return "framebuffer_undefined";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        return "framebuffer_incomplete_attach";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        return "framebuffer_incomplete_missing_attach";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        return "framebuffer_incomplete_draw";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        return "framebuffer_incomplete_read";
    case GL_FRAMEBUFFER_UNSUPPORTED:
        return "framebuffer_unsupported";
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        return "framebuffer_incomplete_multisample";
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        return "framebuffer_incomplete_layer_targets";
    }
    return "framebuffer_no_error";
}
