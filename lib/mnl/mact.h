#ifndef __MACT_H__
#define __MACT_H__

#include "mheads.h"

__BEGIN_DECLS

NEOERR* mact_init();
NEOERR* mact_on(SDL_Event e, bool *running);
void    mact_finish();

__END_DECLS
#endif  /* __MACT_H__ */
