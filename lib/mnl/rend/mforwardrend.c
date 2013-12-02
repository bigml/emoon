#include "mheads.h"

#define MAX_LIGHTS 32

typedef struct {
    GLuint fbo;
    GLuint rbo;                 /* depth rend buffer */
    GLuint crbo;                /* color attachment depth rend buffer
                                   (used only on multisample) */
    GLuint res_fbo;             /* multisample result fbo (used only on multisample) */
    GLuint tex;

    CameraEntity *cam;
    LightEntity *shadow_light;
    GLuint shadow_tex;

    float wmatrix[16];          /* world_matrxi */
    float vmatrix[16];          /* view_matrix */
    float pmatrix[16];          /* projection_matrix */
    float lvmatrix[16];         /* light_view_matrix */
    float lpmatrix[16];         /* light_projection_matrix */

    int tex_counter;
    struct {
        int position;
        int normal;
        int tangent;
        int binormal;
        int uvs;
        int color;
    } attr;                     /* vertex shader attribute indicator */

    int num_lights;
    LightEntity *light[MAX_LIGHTS];

    RendAsset *postmat;         /* used on forward_rend_end() */
} ForwardEntry;

static ForwardEntry *m_render = NULL;

static void mforward_set_mat_attrmap(MatEntry *me)
{
    GLuint prog = me->prog, uniid = -1;
    char uniname[LEN_KEYWORLD];

    if (!me) return;

    m_render->tex_counter = 0;

    for (int i = 0; i < me->items->num; i++) {
        MatItem *item;

        uListGet(me->items, i, (void**)&item);

        snprintf(uniname, sizeof(uniname), "ATTR.%s", item->name);
        uniid = glGetUniformLocation(prog, uniname);

        if (uniid == -1) goto texture;

        if (item->type == MAT_TYPE_INT)
            glUniform1i(uniid, item->as.i);
        if (item->type == MAT_TYPE_FLOAT)
            glUniform1f(uniid, item->as.f);
        if (item->type == MAT_TYPE_VEC2)
            glUniform2f(uniid, item->as.v2.x, item->as.v2.y);
        if (item->type == MAT_TYPE_VEC3)
            glUniform3f(uniid, item->as.v3.x, item->as.v3.y, item->as.v3.z);
        if (item->type == MAT_TYPE_VEC4)
            glUniform4f(uniid, item->as.v4.x, item->as.v4.y,
                        item->as.v4.z, item->as.v4.z);
    texture:
        if (item->type == MAT_TYPE_TEXTURE ||
            item->type == MAT_TYPE_TEXTURE_3D) {
            snprintf(uniname, sizeof(uniname), "MAP.%s", item->name);
            uniid = glGetUniformLocation(prog, uniname);

            if (uniid == -1) continue;

            TexAsset *a = (TexAsset*)item->as.a;

            glActiveTexture(GL_TEXTURE0 + m_render->tex_counter);
            if (item->type == MAT_TYPE_TEXTURE) {
                glBindTexture(GL_TEXTURE_2D, a->tex);
                glEnable(GL_TEXTURE_2D);
            } else {
                glBindTexture(GL_TEXTURE_3D, a->tex);
                glEnable(GL_TEXTURE_3D);
            }
            glUniform1i(uniid, m_render->tex_counter);
            m_render->tex_counter++;
        }
        MGL_CHECK_ERROR();
    }

    uniid = glGetUniformLocation(prog, "MAP.shadow");
    if (m_render->shadow_tex && uniid != -1) {
        glUniform1i(uniid, m_render->tex_counter);
        glActiveTexture(GL_TEXTURE0 + m_render->tex_counter);
        glBindTexture(GL_TEXTURE_2D, m_render->shadow_tex);
        glEnable(GL_TEXTURE_2D);
        m_render->tex_counter++;
    }
}

static void mforward_use_mat_entry(StaticEntity *e, MatEntry *me)
{
    GLuint prog = me->prog, uniid;
    char uniname[LEN_KEYWORLD];

    glUseProgram(prog);
    MGL_CHECK_ERROR();

    /*
     * vertex shader
     *   I. atribute
     *   I. matrix
     */
    m_render->attr.position = glGetAttribLocation(prog, "POSITION");
    m_render->attr.normal   = glGetAttribLocation(prog, "NORMAL");
    m_render->attr.tangent  = glGetAttribLocation(prog, "TANGENT");
    m_render->attr.binormal = glGetAttribLocation(prog, "BINORMAL");
    m_render->attr.uvs      = glGetAttribLocation(prog, "UVS");
    m_render->attr.color    = glGetAttribLocation(prog, "COLOR");

    SET_UNIFORM_MATRIX("WORLD_MATRIX",      m_render->wmatrix);
    SET_UNIFORM_MATRIX("VIEW_MATRIX",       m_render->vmatrix);
    SET_UNIFORM_MATRIX("PROJ_MATRIX",       m_render->pmatrix);
    SET_UNIFORM_MATRIX("LIGHT_VIEW_MATRIX", m_render->lvmatrix);
    SET_UNIFORM_MATRIX("LIGHT_PROJ_MATRIX", m_render->lpmatrix);

    /*
     * fragment shader
     *   I. uniform CAMERA
     *   I. uniform ATTR
     *   I. uniform MAP
     *   I. uniform LIGHT
     */
    SET_UNIFORM_3F("CAMERA_POS", m_render->cam->base.position);

    mforward_set_mat_attrmap(me);

    SET_UNIFORM_1I("NUM_LIGHTS", m_render->num_lights);

    for (int i = 0; i < m_render->num_lights; i++) {
        snprintf(uniname, sizeof(uniname), "LIGHT[%d].power", i);
        SET_UNIFORM_1F(uniname, m_render->light[i]->power);

        snprintf(uniname, sizeof(uniname), "LIGHT[%d].falloff", i);
        SET_UNIFORM_1F(uniname, m_render->light[i]->falloff);

        snprintf(uniname, sizeof(uniname), "LIGHT[%d].position", i);
        SET_UNIFORM_3F(uniname, m_render->light[i]->base.position);

        snprintf(uniname, sizeof(uniname), "LIGHT[%d].target", i);
        SET_UNIFORM_3F(uniname, m_render->light[i]->target);

        snprintf(uniname, sizeof(uniname), "LIGHT[%d].diffuse", i);
        SET_UNIFORM_3F(uniname, m_render->light[i]->diffuse);

        snprintf(uniname, sizeof(uniname), "LIGHT[%d].ambient", i);
        SET_UNIFORM_3F(uniname, m_render->light[i]->ambient);

        snprintf(uniname, sizeof(uniname), "LIGHT[%d].specular", i);
        SET_UNIFORM_3F(uniname, m_render->light[i]->specular);
    }

    MGL_CHECK_ERROR();
}

static void mforward_disuse_mat_entry()
{
    while (m_render->tex_counter > 0) {
        m_render->tex_counter--;
        glActiveTexture(GL_TEXTURE0 + m_render->tex_counter);
        glDisable(GL_TEXTURE_3D);
        glDisable(GL_TEXTURE_2D);
    }

    glUseProgram(0);
}


static void mforward_bind_attributes(GLsizei stride)
{
    /*
     * vertex layout used by glDrawElements()
     */
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, stride, (void*)0);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, stride, (void*)(sizeof(float) * 3));

    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, stride, (void*)(sizeof(float) * 12));

    glColorPointer(4, GL_FLOAT, stride, (void*)(sizeof(float) * 14));
    glEnableClientState(GL_COLOR_ARRAY);

    /*
     * vertex layout used by GLSL vertex shader
     */
#define SET_VERTEX_ATTR(attr, size, pos)                                \
    if (attr != -1) {                                                   \
        glVertexAttribPointer(attr, size, GL_FLOAT, GL_FALSE,           \
                              stride, (void*)(sizeof(float)*pos));      \
        glEnableVertexAttribArray(attr);                                \
    }

    SET_VERTEX_ATTR(m_render->attr.position, 3, 0);
    SET_VERTEX_ATTR(m_render->attr.normal,   3, 3);
    SET_VERTEX_ATTR(m_render->attr.tangent,  3, 6);
    SET_VERTEX_ATTR(m_render->attr.binormal, 3, 9);
    SET_VERTEX_ATTR(m_render->attr.uvs,      2, 12);
    SET_VERTEX_ATTR(m_render->attr.color,    4, 14);
}

static void mforward_unbind_attributes()
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

#define DISABLE_VERTEX_ATTR(attr)                       \
    if (attr != -1) glDisableVertexAttribArray(attr);

    DISABLE_VERTEX_ATTR(m_render->attr.position);
    DISABLE_VERTEX_ATTR(m_render->attr.normal);
    DISABLE_VERTEX_ATTR(m_render->attr.tangent);
    DISABLE_VERTEX_ATTR(m_render->attr.binormal);
    DISABLE_VERTEX_ATTR(m_render->attr.uvs);
    DISABLE_VERTEX_ATTR(m_render->attr.color);
}

NEOERR* mrend_forwardrend_init(char *basedir)
{
    NEOERR *err;

    if (m_render) return STATUS_OK;

    m_render = calloc(1, sizeof(ForwardEntry));
    if (!m_render) return nerr_raise(NERR_NOMEM, "alloc forward rend");

    m_render->shadow_tex = 0;
    m_render->tex_counter = 0;
    m_render->num_lights = 0;
    m_render->res_fbo = 0;

    int width = mrend_viewport_width();
    int height = mrend_viewport_height();
    int multisample = mrend_get_multisamples();

    /*
     * on multisample:
     *   rbo, tex ==> fbo
     * else:
     *   rbo, crbo ==> fbo, tex ==> res_fbo
     */

    /*
     * tex (final texture)
     */
    glGenTextures(1, &m_render->tex);
    glBindTexture(GL_TEXTURE_2D, m_render->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    MGL_CHECK_ERROR();

    /*
     * rbo (depth rend buffer)
     */
    glGenRenderbuffers(1, &m_render->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_render->rbo);
    if (multisample > 0)
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample,
                                         GL_DEPTH_COMPONENT24, width, height);
    else glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    MGL_CHECK_ERROR();

    /*
     * fbo, rbo => fbo
     */
    glGenFramebuffers(1, &m_render->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_render->fbo);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, m_render->rbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (multisample > 0) {
        /*
         * crbo (color rend buffer) ==> fbo
         */
        glBindFramebuffer(GL_FRAMEBUFFER, m_render->fbo);
        glGenRenderbuffers(1, &m_render->crbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_render->crbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample,
                                         GL_RGBA16F, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER, m_render->crbo);

        MGL_CHECK_FRAMEBUFFERSTATUS();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        /*
         * res_fbo, tex ==> res_fbo
         */
        glGenFramebuffers(1, &m_render->res_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_render->res_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, m_render->tex, 0);
        MGL_CHECK_FRAMEBUFFERSTATUS();
    } else {
        /*
         * tex ==> fbo
         */
        glBindFramebuffer(GL_FRAMEBUFFER, m_render->fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, m_render->tex, 0);
        MGL_CHECK_FRAMEBUFFERSTATUS();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*
     * post material
     */
    err = masset_node_load(basedir, "shaders/forward/post.mat", &m_render->postmat);
    if (err != STATUS_OK) return nerr_pass(err);

    MatAsset *m = (MatAsset*)m_render->postmat;
    MatEntry *me;

    err = uListGet(m->entries, 0, (void**)&me);
    if (err != STATUS_OK) return nerr_pass(err);

    mast_mat_add_item_int(me, "width",  mrend_viewport_width());
    mast_mat_add_item_int(me, "height", mrend_viewport_height());
    mast_mat_add_item_int(me, "msaa", mrend_get_multisamples());
    mast_mat_add_item_texture(me, "diffuse", mast_texture_new(m_render->tex));

    return STATUS_OK;
}

void mrend_forwardrend_set_camera(CameraEntity *c)
{
    m_render->cam = c;
}

void mrend_forwardrend_set_shadow_light(LightEntity *e)
{
    m_render->shadow_light = e;
}

void mrend_forwardrend_set_shadow_texture(GLuint texid)
{
    m_render->shadow_tex = texid;
}

void mrend_forwardrend_add_light(LightEntity *e)
{
    if (m_render->num_lights >= MAX_LIGHTS) {
        mtc_warn("Cannot add extra light. Maxiumum lights reached!");
        return;
    }

    m_render->light[m_render->num_lights++] = e;
}

void mrend_forwardrend_begin()
{
    mat4 viewm;
    mat4 projm;

    if (!m_render) return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_render->fbo);
    glClearColor(0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!m_render->cam) {
        mtc_err("Camera not set yet!");
        return;
    }

    if (!m_render->shadow_light) {
        mtc_err("Light not set yet!");
        return;
    }

    viewm = mentity_camera_view_matrix(m_render->cam);
    projm = mentity_camera_proj_matrix(m_render->cam, mrend_viewport_ratio());
    mat4_to_array(viewm, m_render->vmatrix);
    mat4_to_array(projm, m_render->pmatrix);

    viewm = mentity_light_view_matrix(m_render->shadow_light);
    projm = mentity_light_proj_matrix(m_render->shadow_light);
    mat4_to_array(viewm, m_render->lvmatrix);
    mat4_to_array(projm, m_render->lpmatrix);

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
    MatAsset *m;

    VbufferSurface *s;
    MatEntry *me;

    if (!ep) return;

    e = (StaticEntity*)ep;
    r = (VbufferAsset*)e->base.rendable;
    m = (MatAsset*)e->base.material;

    if (!r || !m) return;

    //mtc_dbg("rend static object %s", e->base.name);

    mat4_to_array(mat4_world(e->base.position, e->scale, e->rotation), m_render->wmatrix);

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
    MatAsset *m = (MatAsset*)m_render->postmat;
    MatEntry *me;
    NEOERR *err;

    /*
     * multisample
     */
    if (m_render->res_fbo > 0) {
        int width = mrend_viewport_width();
        int height = mrend_viewport_height();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_render->fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_render->res_fbo);
        glBlitFramebuffer(0, 0, width, height,
                          0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    err = uListGet(m->entries, 0, (void**)&me);
    RETURN_NOK(err);

    glUseProgram(me->prog);
    MGL_CHECK_ERROR();

    mforward_set_mat_attrmap(me);

    mgl_push_matrix();

    mgl_rend_plane();
    MGL_CHECK_ERROR();

    mgl_pop_matrix();

    mforward_disuse_mat_entry();

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

    if (m_render->res_fbo > 0) {
        glDeleteRenderbuffers(1, &m_render->crbo);
        glDeleteFramebuffers(1, &m_render->res_fbo);
    }

    free(m_render);
    m_render = NULL;
}
