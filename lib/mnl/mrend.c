#include "mheads.h"

static RendEntry *m_render = NULL;

NEOERR* mrend_init(char *title, int x, int y, int w, int h, int flags, char *basedir)
{
    NEOERR *err;

    if (m_render) return nerr_raise(NERR_ASSERT, "rend already inited");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return nerr_raise(NERR_ASSERT, "init sdl %s", SDL_GetError());

    if (TTF_Init() == -1)
        return nerr_raise(NERR_ASSERT, "init ttf %s", TTF_GetError());

    m_render = calloc(1, sizeof(RendEntry));
    if (!m_render) return nerr_raise(NERR_NOMEM, "alloc render entry");

    m_render->multisamples = 4;
    m_render->multisamplesbuffs = 1;
    m_render->width = w;
    m_render->height = h;

    m_render->win = SDL_CreateWindow(title, x, y, w, h, flags);
    if (!m_render->win) goto error;

    m_render->render = SDL_CreateRenderer(m_render->win, -1,
                                          SDL_RENDERER_ACCELERATED |
                                          SDL_RENDERER_PRESENTVSYNC);
    if (!m_render->render) goto error;

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, m_render->multisamples);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, m_render->multisamplesbuffs);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    msdl_load_extensions();
    msdl_trace_info();

    err = masset_init();
    if (err != STATUS_OK) return nerr_pass(err);

    err = mentity_init();
    if (err != STATUS_OK) return nerr_pass(err);

    err = mrend_shadowmap_init(basedir, m_render);
    if (err != STATUS_OK) return nerr_pass(err);

    err = mrend_forwardrend_init(basedir, m_render);
    if (err != STATUS_OK) return nerr_pass(err);

    return STATUS_OK;

error:
    if (m_render->win) SDL_DestroyWindow(m_render->win);
    free(m_render);
    m_render = NULL;

    return nerr_raise(NERR_ASSERT, "sdl error %s", SDL_GetError());
}

NEOERR* mrend_update()
{
    return STATUS_OK;
}

NEOERR* mrend_rend()
{
    RendEntity *e;
    char *key = NULL;
    HASH *eh;

    eh = hash_lookup(g_datah, ENTITY_KEY);

    MCS_NOT_NULLA(eh);

    /*
     * shadow mapper
     */
    mrend_shadowmap_begin();
    e = hash_next(eh, (void**)&key);
    while (e) {
        mrend_shadowmap_rend_static(e);

        e = hash_next(eh, (void**)&key);
    }
    mrend_shadowmap_end();

    /*
     * forward render
     */
    mrend_forwardrend_begin();
    key = NULL;
    e = hash_next(eh, (void**)&key);
    while (e) {
        mrend_forwardrend_rend_static(e);

        e = hash_next(eh, (void**)&key);
    }
    mrend_forwardrend_end();

    return STATUS_OK;
}

void mrend_present()
{
    SDL_RenderPresent(m_render->render);
}

void mrend_finish()
{
    if (!m_render) return;

    mrend_shadowmap_finish();
    mrend_forwardrend_finish();

    SDL_DestroyRenderer(m_render->render);
    SDL_DestroyWindow(m_render->win);
    SDL_Quit();

    free(m_render);
    m_render = NULL;
}


int mrend_viewport_width()
{
    return m_render->width;
}

int mrend_viewport_height()
{
    return m_render->height;
}

float mrend_viewport_ratio()
{
    return (float)m_render->height / (float)m_render->width;
}

void mrend_viewport_screenshot(char *name)
{
    char fname[PATH_MAX];
    int w, h;
    NEOERR *err;

    w = m_render->width;
    h = m_render->height;

    ImageAsset *i;

    err = mast_image_load_from_buffer(w, h, GL_COLOR_ATTACHMENT0, (RendAsset**)&i);
    RETURN_NOK(err);

    char tm[LEN_TM] = {0};
    mutil_getdatetime(tm, sizeof(tm), "%Y-%m-%d %H:%M:%S", time(NULL));
    snprintf(fname, sizeof(fname), "./%s_%s.tga", name, tm);

    err = mast_image_write_to_file(i, fname);
    RETURN_NOK(err);

    mast_image_unload(i);
}
