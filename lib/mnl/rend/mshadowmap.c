#include "mheads.h"

typedef struct {
    GLuint fbo;
    GLuint dbo;
    GLuint tex;

    float wmatrix[16];          /* world_matrxi */
    float vmatrix[16];          /* view_matrix */
    float pmatrix[16];          /* projection_matrix */

    LightEntity *light;

    RendAsset *dmat;       /* depth material */
} ShadowEntry;

static ShadowEntry *m_render = NULL;

static void mshadow_use_mat_entry(StaticEntity *e, MatEntry *me)
{
    GLuint prog = me->prog, uniid;

    glUseProgram(prog);
    MGL_CHECK_ERROR();

    SET_UNIFORM_MATRIX("WORLD_MATRIX", m_render->wmatrix);
    SET_UNIFORM_MATRIX("VIEW_MATRIX",  m_render->vmatrix);
    SET_UNIFORM_MATRIX("PROJ_MATRIX",  m_render->pmatrix);
}

static void mshadow_disuse_mat_entry()
{
    glUseProgram(0);
}

static void mshadow_bind_attributes(GLsizei stride)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, stride, (void*)0);
}

static void mshadow_unbind_attributes()
{
    glDisableClientState(GL_VERTEX_ARRAY);
}


NEOERR* mrend_shadowmap_init(char *basedir, LightEntity *light)
{
    NEOERR *err;

    m_render = calloc(1, sizeof(ShadowEntry));
    if (!m_render) return nerr_raise(NERR_NOMEM, "alloc shadowmap rend");

    m_render->light = light;

    int width = light->shadow_map_width;
    int height = light->shadow_map_height;

    /*
     * texture object to store result depth
     */
    glGenTextures(1, &m_render->tex);
    glBindTexture(GL_TEXTURE_2D, m_render->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glBindTexture(GL_TEXTURE_2D, 0);
    MGL_CHECK_ERROR();


    /*
     * render buffer object to store middle depth info
     */
    glGenRenderbuffers(1, &m_render->dbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_render->dbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    MGL_CHECK_ERROR();

    /*
     * frame buffer object
     */
    glGenFramebuffers(1, &m_render->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_render->fbo);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, m_render->dbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, m_render->tex, 0);

    MGL_CHECK_FRAMEBUFFERSTATUS();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*
     * shadow map shader materail
     */
    err = masset_node_load(basedir, "shaders/depth.mat", &m_render->dmat);
    if (err != STATUS_OK) return nerr_pass(err);

    return STATUS_OK;
}

void mrend_shadowmap_begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_render->fbo);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0,
               m_render->light->shadow_map_width,
               m_render->light->shadow_map_height);

    mat4 viewm = mentity_light_view_matrix(m_render->light);
    mat4 projm = mentity_light_proj_matrix(m_render->light);

    mat4_to_array(viewm, m_render->vmatrix);
    mat4_to_array(projm, m_render->pmatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_render->vmatrix);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_render->pmatrix);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
}

void mrend_shadowmap_rend_static(RendEntity *ep)
{
    StaticEntity *e;
    VbufferAsset *r;
    MatAsset *m;

    VbufferSurface *s;
    MatEntry *me;

    if (!ep) return;

    e = (StaticEntity*)ep;
    r = (VbufferAsset*)e->base.rendable;
    m = (MatAsset*)m_render->dmat;

    if (!r || !m || !e->base.cast_shadows) return;

    mat4_to_array(mat4_world(e->base.position, e->scale, e->rotation), m_render->wmatrix);

    uListGet(m->entries, 0, (void**)&me);
    mshadow_use_mat_entry(e, me);

    for (int i = 0; i < r->num_surfaces; i++) {
        s = r->surfaces[i];

        glBindBuffer(GL_ARRAY_BUFFER, s->vertex_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->triangle_vbo);

        mshadow_bind_attributes(sizeof(float) * 18);
        glDrawElements(GL_TRIANGLES, s->num_triangles * 3, GL_UNSIGNED_INT, (void*)0);
        mshadow_unbind_attributes();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        MGL_CHECK_ERROR();
    }

    mshadow_disuse_mat_entry();
}

void mrend_shadowmap_end()
{
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, mrend_viewport_width(), mrend_viewport_height());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void mrend_shadowmap_finish()
{
    if (!m_render) return;

    glDeleteFramebuffers(1, &m_render->fbo);
    glDeleteRenderbuffers(1, &m_render->dbo);
    glDeleteTextures(1, &m_render->tex);

    free(m_render);
    m_render = NULL;
}

GLuint mrend_shadowmap_depth_texture()
{
    return m_render->tex;
}
