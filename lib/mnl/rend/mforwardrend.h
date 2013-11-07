#ifndef __MFORWARDREND_H__
#define __MFORWARDREND_H__

#include "mheads.h"

__BEGIN_DECLS

NEOERR* mrend_forwardrend_init(char *basedir, RendEntry *r);
void    mrend_forwardrend_set_camera(void *c);
void    mrend_forwardrend_begin();
void    mrend_forwardrend_rend_static(RendEntity *e);
void    mrend_forwardrend_end();
void    mrend_forwardrend_finish();

__END_DECLS
#endif  /* MFORWARDREND_H__ */
