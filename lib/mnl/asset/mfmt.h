#ifndef __MASSFMT_H__
#define __MASSFMT_H__

#include "mheads.h"

__BEGIN_DECLS

void mfmt_obj_reset_local();

void mfmt_obj_parse_line(char *line, int *vindex,
                         vertex_list *vlist, int_list *tlist,
                         vertex_list *vdata, vertex_hashtable *vhash,
                         bool *has_normal_data, bool *has_texcoord_data);

__END_DECLS
#endif    /* __MASSFMT_H__ */
