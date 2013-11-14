#ifndef __MASSETIMAGE_H__
#define __MASSETIMAGE_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct {
    RendAsset base;

    int width;
    int height;
    unsigned char *data;
    int repeat_type;
    int sample_type;
} ImageAsset;

NEOERR* mast_image_load_from_buffer(int w, int h, GLenum mode, RendAsset **a);
NEOERR* mast_image_write_to_file(ImageAsset *i, char *fname);
void    mast_image_unload(void *p);

__END_DECLS
#endif  /* MASSETIMAGE_H__ */
