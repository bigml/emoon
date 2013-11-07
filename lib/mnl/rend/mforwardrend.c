#include "mheads.h"

typedef struct {
    GLuint fbo;
    GLuint rbo;
    GLuint tex;

    RendAsset *mat;
    float vmatrix[16];
    float pmatrix[16];

    void *cam;

    float wordmatrix[16];
} ForwardEntry;

static ForwardEntry *m_render = NULL;

static void mrend_bind_attributes(GLsizei stride)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, stride, (void*)0);
  
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, stride, (void*)(sizeof(float) * 3));
  
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, stride, (void*)(sizeof(float) * 12));
  
    glColorPointer(4, GL_FLOAT, stride, (void*)(sizeof(float) * 14));
    glEnableClientState(GL_COLOR_ARRAY);
}

static void mrend_unbind_attributes()
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);  
    glDisableClientState(GL_COLOR_ARRAY);  
}

NEOERR* mrend_forwardrend_init(char *basedir, RendEntry *r)
{
    NEOERR *err;

    glClearColor(0.2, 0.2, 0.2, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (m_render) return STATUS_OK;
    
    m_render = calloc(1, sizeof(ForwardEntry));
    if (!m_render) return nerr_raise(NERR_NOMEM, "alloc forward rend");

    err = masset_node_load(basedir, "shaders/forward/apost.mat", &m_render->mat);
    if (err != STATUS_OK) return nerr_pass(err);
    
    glGenFramebuffers(1, &m_render->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_render->fbo);

    glGenRenderbuffers(1, &m_render->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_render->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, r->width, r->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, m_render->rbo);

    glGenTextures(1, &m_render->tex);
    glBindTexture(GL_TEXTURE_2D, m_render->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, r->width, r->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_render->tex, 0);

    MGL_CHECK_FRAMEBUFFERSTATUS();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return STATUS_OK;
}

void mrend_forwardrend_set_camera(void *c)
{
    m_render->cam = c;
}

void mrend_forwardrend_begin()
{
    if (!m_render) return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_render->fbo);

    if (!m_render->cam) {
        mtc_err("Camera not set yet!");
        return;
    }

    mat4 viewm;
    mat4 projm;
    
    //viewm = camera_view_matrix(m_render->cam);
    //projm = camera_proj_matrix(m_render->cam);

    mat4_to_array(viewm, m_render->vmatrix);
    mat4_to_array(projm, m_render->pmatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_render->vmatrix);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_render->pmatrix);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void mrend_forwardrend_rend_static(RendEntity *ep)
{
    StaticEntity *e;
    VbufferAsset *r;
    VbufferSurface *s;
    MatAsset *m;
    MatEntry *me;

    if (!ep) return;

    e = (StaticEntity*)ep;
    r = (VbufferAsset*)e->base.rendable;
    m = (MatAsset*)e->base.material;

    if (!r || !m) return;

    //mat4_to_array(mat4_word(e->position, e->scale, e->rotation), m_wordmatrix);

    for (int i = 0; i < r->num_surfaces; i++) {
        s = r->surfaces[i];
        uListGet(m->entries, 0, (void**)&me);

        mtc_dbg("rend %d %d %d %d", i, s->vertex_vbo, s->triangle_vbo, s->num_triangles);

        //GLint receive_shadows = glGetUniformLocation(me->prog, "receive_shadows");
        //if (receive_shadows != -1) glUniform1i(receive_shadows, e->base.receive_shadows);

        glBindBuffer(GL_ARRAY_BUFFER, s->vertex_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->triangle_vbo);

        mrend_bind_attributes(sizeof(float) * 18);
        glDrawElements(GL_TRIANGLES, s->num_triangles * 3, GL_UNSIGNED_INT, (void*)0);
        mrend_unbind_attributes();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void mrend_forwardrend_end()
{
    MatAsset *m = (MatAsset*)m_render->mat;
    MatEntry *e;
    NEOERR *err;
    
    err = uListGet(m->entries, 0, (void**)&e);
    RETURN_NOK(err);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(e->prog);

    mgl_push_matrix();

    mgl_rend_texture(m_render->tex);
    
    mgl_pop_matrix();

    glUseProgram(0);
}

void mrend_forwardrend_finish()
{
    if (!m_render) return;
    
    glDeleteFramebuffers(1, &m_render->fbo);
    glDeleteRenderbuffers(1, &m_render->rbo);
    glDeleteTextures(1, &m_render->tex);
    
    free(m_render);
    m_render = NULL;
}
