#include "mheads.h"

static int m_repeat_tile = 0;
//static int m_repeat_clamp = 1;
//static int m_repeat_mirror = 2;

static int m_sample_linear = 0;
//static int m_sample_nearest = 1;

static vec4 mimage_get_pixel(ImageAsset *i, int u, int v)
{
    v = i->height - v - 1;
  
    if (u < 0 || v < 0 || u >= i->width || v >=i->height) {
        mtc_err("Image %s Out of Bounds", i->base.name);
        return vec4_zero();
    }
  
    float r = (float)i->data[u * 4 + v * i->width * 4 + 0] / 255;
    float g = (float)i->data[u * 4 + v * i->width * 4 + 1] / 255;
    float b = (float)i->data[u * 4 + v * i->width * 4 + 2] / 255;
    float a = (float)i->data[u * 4 + v * i->width * 4 + 3] / 255;

    return vec4_new(r,g,b,a);
}

static void mimage_set_pixel(ImageAsset *i, int u, int v, vec4 color)
{
    v = i->height - v - 1;
    i->data[u * 4 + v * i->width * 4 + 0] = (color.x * 255);
    i->data[u * 4 + v * i->width * 4 + 1] = (color.y * 255);
    i->data[u * 4 + v * i->width * 4 + 2] = (color.z * 255);
    i->data[u * 4 + v * i->width * 4 + 3] = (color.w * 255);
}

static void mimage_bgr_to_rgb(ImageAsset *i)
{
    for (int x = 0; x < i->width; x++) {
        for (int y = 0; y < i->height; y++) {
            vec4 pix = mimage_get_pixel(i, x, y);
            vec4 rev = vec4_new(pix.z, pix.y, pix.x, pix.w);
            
            mimage_set_pixel(i, x, y, rev);
        }
    }
}

NEOERR* mast_image_load_from_buffer(int w, int h, GLenum mode, RendAsset **a)
{
    ImageAsset *ast = calloc(1, sizeof(ImageAsset));
    if (!ast) return nerr_raise(NERR_NOMEM, "alloc image");

    ast->data = malloc(sizeof(unsigned char) * w * h * 4);
    ast->width = w;
    ast->height = h;
    ast->repeat_type = m_repeat_tile;
    ast->sample_type = m_sample_linear;

    glReadBuffer(mode);
    MGL_CHECK_ERROR();
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, ast->data);
    MGL_CHECK_ERROR();

    *a = (RendAsset*)ast;

    return STATUS_OK;
}

NEOERR* mast_image_write_to_file(ImageAsset *i, char *fname)
{
    mimage_bgr_to_rgb(i);
  
    int xa = i->width % 256;
    int xb = (i->width-xa) / 256;

    int ya = i->height % 256;
    int yb = (i->height-ya) / 256;
    unsigned char header[18] =
        {0,0,2,0,0,0,0,0,0,0,0,0,(char)xa,(char)xb,(char)ya,(char)yb,32,0};
  
    SDL_RWops* file = SDL_RWFromFile(fname, "wb");
    if (!file) return nerr_raise(NERR_IO, "open file %s for write", fname);
  
    SDL_RWwrite(file, header, sizeof(header), 1);
    SDL_RWwrite(file, i->data, i->width * i->height * 4, 1);
    SDL_RWclose(file);

    mimage_bgr_to_rgb(i);

    return STATUS_OK;
}

void mast_image_unload(void *p)
{
    if (!p) return;

    ImageAsset *a = p;
    
    free(a->data);
}
