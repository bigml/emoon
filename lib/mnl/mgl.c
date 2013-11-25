#include "mheads.h"

void mgl_push_matrix()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
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

    glGenerateMipmap(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, -1.0,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, -1.0,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0,  1.0,  0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0,  1.0,  0.0f);
    glEnd();

    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void mgl_pop_matrix()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


char* mgl_error_str(GLenum error)
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

char* mgl_error_str_framebuffer(GLenum error)
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

char* mgl_format_str(GLenum format)
{
    switch(format) {
    case GL_STENCIL_INDEX:      // 0x1901
        return "GL_STENCIL_INDEX";
    case GL_DEPTH_COMPONENT:    // 0x1902
        return "GL_DEPTH_COMPONENT";
    case GL_ALPHA:              // 0x1906
        return "GL_ALPHA";
    case GL_RGB:                // 0x1907
        return "GL_RGB";
    case GL_RGBA:               // 0x1908
        return "GL_RGBA";
    case GL_LUMINANCE:          // 0x1909
        return "GL_LUMINANCE";
    case GL_LUMINANCE_ALPHA:    // 0x190A
        return "GL_LUMINANCE_ALPHA";
    case GL_R3_G3_B2:           // 0x2A10
        return "GL_R3_G3_B2";
    case GL_ALPHA4:             // 0x803B
        return "GL_ALPHA4";
    case GL_ALPHA8:             // 0x803C
        return "GL_ALPHA8";
    case GL_ALPHA12:            // 0x803D
        return "GL_ALPHA12";
    case GL_ALPHA16:            // 0x803E
        return "GL_ALPHA16";
    case GL_LUMINANCE4:         // 0x803F
        return "GL_LUMINANCE4";
    case GL_LUMINANCE8:         // 0x8040
        return "GL_LUMINANCE8";
    case GL_LUMINANCE12:        // 0x8041
        return "GL_LUMINANCE12";
    case GL_LUMINANCE16:        // 0x8042
        return "GL_LUMINANCE16";
    case GL_LUMINANCE4_ALPHA4:  // 0x8043
        return "GL_LUMINANCE4_ALPHA4";
    case GL_LUMINANCE6_ALPHA2:  // 0x8044
        return "GL_LUMINANCE6_ALPHA2";
    case GL_LUMINANCE8_ALPHA8:  // 0x8045
        return "GL_LUMINANCE8_ALPHA8";
    case GL_LUMINANCE12_ALPHA4: // 0x8046
        return "GL_LUMINANCE12_ALPHA4";
    case GL_LUMINANCE12_ALPHA12:// 0x8047
        return "GL_LUMINANCE12_ALPHA12";
    case GL_LUMINANCE16_ALPHA16:// 0x8048
        return "GL_LUMINANCE16_ALPHA16";
    case GL_INTENSITY:          // 0x8049
        return "GL_INTENSITY";
    case GL_INTENSITY4:         // 0x804A
        return "GL_INTENSITY4";
    case GL_INTENSITY8:         // 0x804B
        return "GL_INTENSITY8";
    case GL_INTENSITY12:        // 0x804C
        return "GL_INTENSITY12";
    case GL_INTENSITY16:        // 0x804D
        return "GL_INTENSITY16";
    case GL_RGB4:               // 0x804F
        return "GL_RGB4";
    case GL_RGB5:               // 0x8050
        return "GL_RGB5";
    case GL_RGB8:               // 0x8051
        return "GL_RGB8";
    case GL_RGB10:              // 0x8052
        return "GL_RGB10";
    case GL_RGB12:              // 0x8053
        return "GL_RGB12";
    case GL_RGB16:              // 0x8054
        return "GL_RGB16";
    case GL_RGBA2:              // 0x8055
        return "GL_RGBA2";
    case GL_RGBA4:              // 0x8056
        return "GL_RGBA4";
    case GL_RGB5_A1:            // 0x8057
        return "GL_RGB5_A1";
    case GL_RGBA8:              // 0x8058
        return "GL_RGBA8";
    case GL_RGB10_A2:           // 0x8059
        return "GL_RGB10_A2";
    case GL_RGBA12:             // 0x805A
        return "GL_RGBA12";
    case GL_RGBA16:             // 0x805B
        return "GL_RGBA16";
    case GL_DEPTH_COMPONENT16:  // 0x81A5
        return "GL_DEPTH_COMPONENT16";
    case GL_DEPTH_COMPONENT24:  // 0x81A6
        return "GL_DEPTH_COMPONENT24";
    case GL_DEPTH_COMPONENT32:  // 0x81A7
        return "GL_DEPTH_COMPONENT32";
    case GL_DEPTH_STENCIL:      // 0x84F9
        return "GL_DEPTH_STENCIL";
        /*
    case GL_RGBA32F:            // 0x8814
        return "GL_RGBA32F";
    case GL_RGB32F:             // 0x8815
        return "GL_RGB32F";
    case GL_RGBA16F:            // 0x881A
        return "GL_RGBA16F";
    case GL_RGB16F:             // 0x881B
        return "GL_RGB16F";
        */
    case GL_DEPTH24_STENCIL8:   // 0x88F0
        return "GL_DEPTH24_STENCIL8";
    }
    return "Unknown Format";
}

char* mgl_get_texture_parameters(GLuint objid)
{
    int width, height, format;
    static char rstr[256];

    if (glIsTexture(objid) == GL_FALSE) return "not texture object";

    glBindTexture(GL_TEXTURE_2D, objid);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
    glBindTexture(GL_TEXTURE_2D, 0);

    snprintf(rstr, sizeof(rstr), "%dx%d, %s", width, height, mgl_format_str(format));

    return rstr;
}

char* mgl_get_renderbuffer_parameters(GLuint objid)
{
    int width, height, format;
    static char rstr[256];
    if (glIsRenderbuffer(objid) == GL_FALSE) return "not renderbuffer object";

    glBindRenderbuffer(GL_RENDERBUFFER, objid);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    snprintf(rstr, sizeof(rstr), "%dx%d, %s", width, height, mgl_format_str(format));

    return rstr;
}
