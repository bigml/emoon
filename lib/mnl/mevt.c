#include "mheads.h"

void mevt_on(SDL_Event e, bool *running)
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
}
