#include "mheads.h"

typedef struct {
    SDL_Window *win;
    SDL_Renderer *render;
} RendEntry;

static RendEntry *m_render = NULL;

NEOERR* mrend_init(char *title, int x, int y, int w, int h, int flags)
{
    if (m_render) return nerr_raise(NERR_ASSERT, "rend already inited");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return nerr_raise(NERR_ASSERT, "init sdl %s", SDL_GetError());

    if (TTF_Init() == -1)
        return nerr_raise(NERR_ASSERT, "init ttf %s", TTF_GetError());

    m_render = calloc(1, sizeof(RendEntry));
    if (!m_render) return nerr_raise(NERR_NOMEM, "alloc render entry");

    m_render->win = SDL_CreateWindow(title, x, y, w, h, flags);
    if (!m_render->win) goto error;

    m_render->render = SDL_CreateRenderer(m_render->win, -1,
                                          SDL_RENDERER_ACCELERATED |
                                          SDL_RENDERER_PRESENTVSYNC);
    if (!m_render->render) goto error;

    glClearDepth(1);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    msdl_trace_info();

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
    return STATUS_OK;
}

void mrend_present()
{
    SDL_RenderPresent(m_render->render);
}

void mrend_finish()
{
    if (!m_render) return;
    
    SDL_DestroyRenderer(m_render->render);
    SDL_DestroyWindow(m_render->win);
    SDL_Quit();
    
    free(m_render);
    m_render = NULL;
}
