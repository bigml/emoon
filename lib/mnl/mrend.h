#ifndef __MREND_H__
#define __MREND_H__

#include "mheads.h"

__BEGIN_DECLS

NEOERR* mrend_init(char *title, int x, int y, int w, int h, int flags);
NEOERR* mrend_update();
NEOERR* mrend_rend();
void    mrend_present();
void    mrend_finish();

__END_DECLS
#endif    /* __MREND_H__ */
