#include "mheads.h"

static SDL_TimerID m_tid = 0;

NEOERR* mact_init()
{
    m_tid = SDL_AddTimer(10, mtimer_tick_callback, NULL);

    return STATUS_OK;
}

NEOERR* mact_on(SDL_Event e, bool *running)
{
    RendEntity *ent;
    char *key;
    HASH *eh;

    eh = hash_lookup(g_datah, ENTITY_KEY);

    MCS_NOT_NULLA(eh);

    key = NULL;
    ent = hash_next(eh, (void**)&key);
    while (ent) {
        if (ent->typeid > ENTITY_TYPE_LIGHT) mentity_node_onact(ent, e, running);

        ent = hash_next(eh, (void**)&key);
    }

    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_ESCAPE:
            *running = false;
            return STATUS_OK;
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
