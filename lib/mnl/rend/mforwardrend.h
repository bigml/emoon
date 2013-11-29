#ifndef __MFORWARDREND_H__
#define __MFORWARDREND_H__

#include "mheads.h"

__BEGIN_DECLS

NEOERR* mrend_forwardrend_init(char *basedir);
void    mrend_forwardrend_begin();
void    mrend_forwardrend_rend_static(RendEntity *e);
void    mrend_forwardrend_end();
void    mrend_forwardrend_finish();

void mrend_forwardrend_set_camera(CameraEntity *c);
void mrend_forwardrend_set_shadow_light(LightEntity *e);
void mrend_forwardrend_set_shadow_texture(GLuint texid);
void mrend_forwardrend_add_light(LightEntity *e);

__END_DECLS
#endif  /* MFORWARDREND_H__ */
