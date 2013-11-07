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
