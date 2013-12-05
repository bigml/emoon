#ifndef __LACT_H__
#define __LACT_H__

#include "mheads.h"

__BEGIN_DECLS

NEOERR* lact_init();
NEOERR* lact_on(SDL_Event e, bool *running);
void    lact_finish();

__END_DECLS
#endif    /* __LACT_H__ */
