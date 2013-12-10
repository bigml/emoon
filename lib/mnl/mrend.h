#ifndef __MREND_H__
#define __MREND_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    SDL_Window *win;
    SDL_Renderer *render;
    int multisamples;
    int multisamplesbuffs;
    int width;
    int height;
} RendEntry;

NEOERR* mrend_init(char *title, int x, int y, int w, int h, int flags);
NEOERR* mrend_update(float dt);
NEOERR* mrend_shadowmap_rend();
NEOERR* mrend_forwardrend_rend();
void    mrend_present();
void    mrend_finish();

int   mrend_viewport_width();
int   mrend_viewport_height();
float mrend_viewport_ratio();
int   mrend_get_multisamples();
void  mrend_viewport_screenshot(char *name);

__END_DECLS
#endif    /* __MREND_H__ */
