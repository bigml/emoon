#ifndef __LREND_H__
#define __LREND_H__

#include "mheads.h"

__BEGIN_DECLS

NEOERR* lrend_init();
NEOERR* lrend_update();
NEOERR* lrend_rend();
void    lrend_finish();

__END_DECLS
#endif    /* __LREND_H__ */
