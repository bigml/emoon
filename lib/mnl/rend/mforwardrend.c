#include "mheads.h"

typedef struct {
    GLuint fbo;
    GLuint rbo;
    GLuint tex;

    RendAsset *mat;
    float vmatrix[16];
    float pmatrix[16];

    void *cam;
    int tex_counter;

    float wordmatrix[16];
} ForwardEntry;

static ForwardEntry *m_render = NULL;

static void mforward_use_mat_entry(StaticEntity *e, MatEntry *me)
{
    float worldmatrix[16];

    glUseProgram(me->prog);
    MGL_CHECK_ERROR();
    
    mat4_to_array(mat4_world(e->base.position, e->scale, e->rotation), worldmatrix);
    GLint world_matrix_u = glGetUniformLocation(me->prog, "world_matrix");
    glUniformMatrix4fv(world_matrix_u, 1, 0, worldmatrix);

    m_render->tex_counter = 0;

    for (int i = 0; i < me->items->num; i++) {
        MatItem *item;

        uListGet(me->items, i, (void**)&item);
        
        GLint loc = glGetUniformLocation(me->prog, item->name);
        
        if (item->type == MAT_TYPE_INT)
            glUniform1i(loc, item->as.i);
        if (item->type == MAT_TYPE_FLOAT)
            glUniform1f(loc, item->as.f);
        if (item->type == MAT_TYPE_VEC2)
            glUniform2f(loc, item->as.v2.x, item->as.v2.y);
        if (item->type == MAT_TYPE_VEC3)
            glUniform3f(loc, item->as.v3.x, item->as.v3.y, item->as.v3.z);
        if (item->type == MAT_TYPE_VEC4)
            glUniform4f(loc, item->as.v4.x, item->as.v4.y,
                        item->as.v4.z, item->as.v4.z);
        if (item->type == MAT_TYPE_TEXTURE) {
            TexAsset *a = (TexAsset*)item->as.a;
            
            glUniform1i(loc, m_render->tex_counter);
            glActiveTexture(GL_TEXTURE0 + m_render->tex_counter);
            glBindTexture(GL_TEXTURE_2D, a->tex);
            glEnable(GL_TEXTURE_2D);
            m_render->tex_counter++;
        }
    }
}

static void mforward_disuse_mat_entry()
{
    while (m_render->tex_counter > 0) {
        m_render->tex_counter--;
        glActiveTexture(GL_TEXTURE0 + m_render->tex_counter);
        glDisable(GL_TEXTURE_2D);
    }

    glUseProgram(0);
}


static void mforward_bind_attributes(GLsizei stride)
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

static void mforward_unbind_attributes()
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);  
    glDisableClientState(GL_COLOR_ARRAY);  
}

NEOERR* mrend_forwardrend_init(char *basedir, RendEntry *r)
{
    NEOERR *err;

    glClearColor(1.0, 1.0, 1.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_render) return STATUS_OK;
    
    m_render = calloc(1, sizeof(ForwardEntry));
    if (!m_render) return nerr_raise(NERR_NOMEM, "alloc forward rend");

    m_render->tex_counter = 0;

    err = masset_node_load(basedir, "shaders/forward/apost.mat", &m_render->mat);
    if (err != STATUS_OK) return nerr_pass(err);

    /*
     * texture object
     */
    glGenTextures(1, &m_render->tex);
    glBindTexture(GL_TEXTURE_2D, m_render->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, r->width, r->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    MGL_CHECK_ERROR();

    /*
     * render buffer object to store depth info
     */
    glGenRenderbuffers(1, &m_render->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_render->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, r->width, r->height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    MGL_CHECK_ERROR();

    /*
     * frame buffer object
     */
    glGenFramebuffers(1, &m_render->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_render->fbo);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, m_render->rbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_render->tex, 0);

    /*
     * check status
     */
    MGL_CHECK_FRAMEBUFFERSTATUS();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return STATUS_OK;
}

void mrend_forwardrend_set_camera(CameraEntity *c)
{
    m_render->cam = c;
}

void mrend_forwardrend_begin()
{
    if (!m_render) return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_render->fbo);

    glClearColor(0.4, 0.4, 0.4, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (!m_render->cam) {
        mtc_err("Camera not set yet!");
        return;
    }

    mat4 viewm = mentity_camera_view_matrix(m_render->cam);
    mat4 projm = mentity_camera_proj_matrix(m_render->cam, mrend_viewport_ratio());

    mat4_to_array(viewm, m_render->vmatrix);
    mat4_to_array(projm, m_render->pmatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_render->vmatrix);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_render->pmatrix);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    MGL_CHECK_ERROR();
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

    for (int i = 0; i < r->num_surfaces; i++) {
        s = r->surfaces[i];
        uListGet(m->entries, min(i, m->entries->num), (void**)&me);

        mforward_use_mat_entry(e, me);

        glBindBuffer(GL_ARRAY_BUFFER, s->vertex_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->triangle_vbo);

        mforward_bind_attributes(sizeof(float) * 18);
        glDrawElements(GL_TRIANGLES, s->num_triangles * 3, GL_UNSIGNED_INT, (void*)0);
        mforward_unbind_attributes();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mforward_disuse_mat_entry();
        MGL_CHECK_ERROR();
    }
}

void mrend_forwardrend_end()
{
    MatAsset *m = (MatAsset*)m_render->mat;
    MatEntry *e;
    NEOERR *err;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    err = uListGet(m->entries, 0, (void**)&e);
    RETURN_NOK(err);
    
    glUseProgram(e->prog);
    MGL_CHECK_ERROR();

    mgl_push_matrix();
    mgl_rend_texture(m_render->tex);
    mgl_pop_matrix();
    MGL_CHECK_ERROR();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    MGL_CHECK_ERROR();

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
