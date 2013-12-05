#include "mheads.h"

static SDL_TimerID m_tid = 0;

NEOERR* mact_init()
{
    m_tid = SDL_AddTimer(10, mtimer_tick_callback, NULL);

    return STATUS_OK;
}

NEOERR* mact_on(SDL_Event e, bool *running)
{
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_ESCAPE:
            *running = false;
            break;
        default:
            break;
        }
    }

    return STATUS_OK;
}

void mact_finish()
{
    if (m_tid != 0) SDL_RemoveTimer(m_tid);
}
