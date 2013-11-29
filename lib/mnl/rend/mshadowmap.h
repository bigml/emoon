#ifndef __MSHADOWMAP_H__
#define __MSHADOWMAP_H__

#include "mheads.h"

__BEGIN_DECLS

NEOERR* mrend_shadowmap_init(char *basedir, LightEntity *light);
void    mrend_shadowmap_begin();
void    mrend_shadowmap_rend_static(RendEntity *e);
void    mrend_shadowmap_end();
void    mrend_shadowmap_finish();

GLuint mrend_shadowmap_depth_texture();

__END_DECLS
#endif  /* MSHADOWMAP_H__ */
