#include "mheads.h"

/* little-endian, of course */
#define DDS_MAGIC 0x20534444


/* DDS_header.dwFlags */
#define DDSD_CAPS                   0x00000001
#define DDSD_HEIGHT                 0x00000002
#define DDSD_WIDTH                  0x00000004
#define DDSD_PITCH                  0x00000008
#define DDSD_PIXELFORMAT            0x00001000
#define DDSD_MIPMAPCOUNT            0x00020000
#define DDSD_LINEARSIZE             0x00080000
#define DDSD_DEPTH                  0x00800000

/* DDS_header.sPixelFormat.dwFlags */
#define DDPF_ALPHAPIXELS            0x00000001
#define DDPF_FOURCC                 0x00000004
#define DDPF_INDEXED                0x00000020
#define DDPF_RGB                    0x00000040

/* DDS_header.sCaps.dwCaps1 */
#define DDSCAPS_COMPLEX             0x00000008
#define DDSCAPS_TEXTURE             0x00001000
#define DDSCAPS_MIPMAP              0x00400000

/* DDS_header.sCaps.dwCaps2 */
#define DDSCAPS2_CUBEMAP            0x00000200
#define DDSCAPS2_CUBEMAP_POSITIVEX  0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX  0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY  0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY  0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ  0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ  0x00008000
#define DDSCAPS2_VOLUME             0x00200000

#define D3DFMT_DXT1     0x31545844    /* DXT1 compression texture format */
#define D3DFMT_DXT2     0x32545844    /* DXT2 compression texture format */
#define D3DFMT_DXT3     0x33545844    /* DXT3 compression texture format */
#define D3DFMT_DXT4     0x34545844    /* DXT4 compression texture format */
#define D3DFMT_DXT5     0x35545844    /* DXT5 compression texture format */

#define GL_COMPRESSED_RGBA_S3TC_DXT1 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5 0x83F3
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_SHORT_5_6_5 0x8363

#define PF_IS_DXT1(pf)                          \
    ((pf.dwFlags & DDPF_FOURCC) &&              \
     (pf.dwFourCC == D3DFMT_DXT1))

#define PF_IS_DXT3(pf)                          \
    ((pf.dwFlags & DDPF_FOURCC) &&              \
     (pf.dwFourCC == D3DFMT_DXT3))

#define PF_IS_DXT5(pf)                          \
    ((pf.dwFlags & DDPF_FOURCC) &&              \
     (pf.dwFourCC == D3DFMT_DXT5))

#define PF_IS_BGRA8(pf)                         \
    ((pf.dwFlags & DDPF_RGB) &&                 \
     (pf.dwFlags & DDPF_ALPHAPIXELS) &&         \
     (pf.dwRGBBitCount == 32) &&                \
     (pf.dwRBitMask == 0xff0000) &&             \
     (pf.dwGBitMask == 0xff00) &&               \
     (pf.dwBBitMask == 0xff) &&                 \
     (pf.dwAlphaBitMask == 0xff000000U))

#define PF_IS_BGR8(pf)                          \
    ((pf.dwFlags & DDPF_RGB) &&                 \
     !(pf.dwFlags & DDPF_ALPHAPIXELS) &&        \
     (pf.dwRGBBitCount == 24) &&                \
     (pf.dwRBitMask == 0xff0000) &&             \
     (pf.dwGBitMask == 0xff00) &&               \
     (pf.dwBBitMask == 0xff))

#define PF_IS_BGR5A1(pf)                        \
    ((pf.dwFlags & DDPF_RGB) &&                 \
     (pf.dwFlags & DDPF_ALPHAPIXELS) &&         \
     (pf.dwRGBBitCount == 16) &&                \
     (pf.dwRBitMask == 0x00007c00) &&           \
     (pf.dwGBitMask == 0x000003e0) &&           \
     (pf.dwBBitMask == 0x0000001f) &&           \
     (pf.dwAlphaBitMask == 0x00008000))

#define PF_IS_BGR565(pf)                        \
    ((pf.dwFlags & DDPF_RGB) &&                 \
     !(pf.dwFlags & DDPF_ALPHAPIXELS) &&        \
     (pf.dwRGBBitCount == 16) &&                \
     (pf.dwRBitMask == 0x0000f800) &&           \
     (pf.dwGBitMask == 0x000007e0) &&           \
     (pf.dwBBitMask == 0x0000001f))

#define PF_IS_INDEX8(pf)                        \
    ((pf.dwFlags & DDPF_INDEXED) &&             \
     (pf.dwRGBBitCount == 8))


typedef struct {
    unsigned int    dwMagic;
    unsigned int    dwSize;
    unsigned int    dwFlags;
    unsigned int    dwHeight;
    unsigned int    dwWidth;
    unsigned int    dwPitchOrLinearSize;
    unsigned int    dwDepth;
    unsigned int    dwMipMapCount;
    unsigned int    dwReserved1[ 11 ];

    /* DDPIXELFORMAT */
    struct {
        unsigned int    dwSize;
        unsigned int    dwFlags;
        unsigned int    dwFourCC;
        unsigned int    dwRGBBitCount;
        unsigned int    dwRBitMask;
        unsigned int    dwGBitMask;
        unsigned int    dwBBitMask;
        unsigned int    dwAlphaBitMask;
    } sPixelFormat;

    /* DDCAPS2 */
    struct {
        unsigned int    dwCaps1;
        unsigned int    dwCaps2;
        unsigned int    dwDDSX;
        unsigned int    dwReserved;
    } sCaps;

    unsigned int dwReserved2;
} DDS_header ;

typedef struct {

    int compressed;
    int swap;
    int palette;
    int divSize;
    int blockBytes;
    GLenum internalFormat;
    GLenum externalFormat;
    GLenum type;

} DdsLoadInfo;

static bool is_power_of_two(unsigned int x)
{
    while (((x % 2) == 0) && x > 1) /* While x is even and > 1 */
        x /= 2;
    return (x == 1);
}

static void mtex_set_filtering_anisotropic(GLuint t)
{
  float max;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);

  glBindTexture(GL_TEXTURE_2D, t);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max);
}

static TexAsset* mtex_node_new()
{
    TexAsset *tnode = calloc(1, sizeof(TexAsset));
    if (!tnode) return NULL;

    return tnode;
}

NEOERR* mast_dds_load(char *dir, char *name, RendAsset **a)
{
    char fname[PATH_MAX], *buf, *pos;
    NEOERR *err;

    DdsLoadInfo loadInfoDXT1 = {
        1, 0, 0, 4, 8, GL_COMPRESSED_RGBA_S3TC_DXT1
    };
    DdsLoadInfo loadInfoDXT3 = {
        1, 0, 0, 4, 16, GL_COMPRESSED_RGBA_S3TC_DXT3
    };
    DdsLoadInfo loadInfoDXT5 = {
        1, 0, 0, 4, 16, GL_COMPRESSED_RGBA_S3TC_DXT5
    };
    DdsLoadInfo loadInfoBGRA8 = {
        0, 0, 0, 1, 4, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE
    };
    DdsLoadInfo loadInfoBGR8 = {
        0, 0, 0, 1, 3, GL_RGB8, GL_BGR, GL_UNSIGNED_BYTE
    };
    DdsLoadInfo loadInfoBGR5A1 = {
        0, 1, 0, 1, 2, GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV
    };
    DdsLoadInfo loadInfoBGR565 = {
        0, 1, 0, 1, 2, GL_RGB5, GL_RGB, GL_UNSIGNED_SHORT_5_6_5
    };
    DdsLoadInfo loadInfoIndex8 = {
        0, 0, 1, 1, 1, GL_RGB8, GL_BGRA, GL_UNSIGNED_BYTE
    };

    if (dir) snprintf(fname, sizeof(fname), "%s%s", dir, name);
    else strncpy(fname, name, sizeof(fname));

    TexAsset *tnode = mtex_node_new();
    if (!tnode) return nerr_raise(NERR_NOMEM, "alloc texture");

    DDS_header hdr;
    int x = 0;
    int y = 0;
    int mipMapCount = 0;
    int totallen = 0;

    err = ne_load_file_len(fname, &buf, &totallen);
    if (err != STATUS_OK) return nerr_pass(err);

    glGenTextures(1, &tnode->tex);
    glBindTexture(GL_TEXTURE_2D, tnode->tex);

    pos = buf;
    memcpy(&hdr, buf, sizeof(hdr));
    pos += sizeof(hdr);
    if (pos - buf > totallen) return nerr_raise(NERR_ASSERT, "file too short");

    if (hdr.dwMagic != DDS_MAGIC || hdr.dwSize != 124 ||
        !(hdr.dwFlags & DDSD_PIXELFORMAT) || !(hdr.dwFlags & DDSD_CAPS) )
        return nerr_raise(NERR_ASSERT, "%s Does not appear to be a .dds file", fname);

    x = hdr.dwWidth;
    y = hdr.dwHeight;

    if (!is_power_of_two(x)) {
        mtc_warn("Texture %s with is %i pixels which is not a power of two!", fname, x);
    }

    if (!is_power_of_two(y)) {
        mtc_warn("Texture %s height is %i pixels which is not a power of two!", fname, y);
    }

    DdsLoadInfo* li = &loadInfoDXT1;

    if (PF_IS_DXT1(hdr.sPixelFormat)) {
        li = &loadInfoDXT1;
    } else if (PF_IS_DXT3(hdr.sPixelFormat)) {
        li = &loadInfoDXT3;
    } else if (PF_IS_DXT5(hdr.sPixelFormat)) {
        li = &loadInfoDXT5;
    } else if (PF_IS_BGRA8(hdr.sPixelFormat)) {
        li = &loadInfoBGRA8;
    } else if (PF_IS_BGR8(hdr.sPixelFormat)) {
        li = &loadInfoBGR8;
    } else if (PF_IS_BGR5A1(hdr.sPixelFormat)) {
        li = &loadInfoBGR5A1;
    } else if (PF_IS_BGR565(hdr.sPixelFormat)) {
        li = &loadInfoBGR565;
    } else if (PF_IS_INDEX8(hdr.sPixelFormat)) {
        li = &loadInfoIndex8;
    } else {
        return nerr_raise(NERR_ASSERT, "%s: Unknown DDS File format type.", fname);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    mipMapCount = (hdr.dwFlags & DDSD_MIPMAPCOUNT) ? hdr.dwMipMapCount : 1;

    int ix, zz;
    GLenum cFormat, format;

    if (li->compressed) {
        size_t size = max(li->divSize, x) / li->divSize *
                      max(li->divSize, y) / li->divSize * li->blockBytes;
        char *data = malloc(size);
        if (!data ) {
            return nerr_raise(NERR_ASSERT, "%s: not contain any data.", fname);
        }

        cFormat = li->internalFormat;
        format = li->internalFormat;

        for( ix = 0; ix < mipMapCount; ++ix ) {
            memcpy(data, pos, size);
            pos += size;
            if (pos - buf > totallen) return nerr_raise(NERR_ASSERT, "file too short");

            glCompressedTexImage2D(GL_TEXTURE_2D, ix, li->internalFormat,
                                   x, y, 0, size, data);
            x = (x+1)>>1;
            y = (y+1)>>1;
            size = max(li->divSize, x) / li->divSize *
                   max(li->divSize, y) / li->divSize * li->blockBytes;
        }
        free(data);
    } else if (li->palette) {
        size_t size = hdr.dwPitchOrLinearSize * y;
        format = li->externalFormat;
        cFormat = li->internalFormat;
        char *data = malloc(size);
        int palette[256];
        int *unpacked = malloc(size * sizeof(int));

        memcpy(palette, pos, 4 * 256);
        pos += 4 * 256;
        if (pos - buf > totallen) return nerr_raise(NERR_ASSERT, "file too short");

        for (ix = 0; ix < mipMapCount; ++ix) {

            memcpy(data, pos, size);
            pos += size;
            if (pos - buf > totallen) return nerr_raise(NERR_ASSERT, "file too short");

            for (zz = 0; zz < size; ++zz) {
                unpacked[zz] = palette[(short)data[zz]];
            }

            glPixelStorei(GL_UNPACK_ROW_LENGTH, y);
            glTexImage2D(GL_TEXTURE_2D, ix, li->internalFormat,
                         x, y, 0, li->externalFormat, li->type, unpacked);
            x = (x+1) >> 1;
            y = (y+1) >> 1;

            size = x * y * li->blockBytes;
        }
        free(data);
        free(unpacked);
    } else {
        if (li->swap ) {
            glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
        }

        size_t size = x * y * li->blockBytes;
        format = li->externalFormat;
        cFormat = li->internalFormat;
        char *data = malloc(size);
        /* fixme: how are MIP maps stored for 24-bit if pitch != ySize*3 ? */
        for (ix = 0; ix < mipMapCount; ++ix) {
            memcpy(data, pos, size);
            pos += size;
            if (pos - buf > totallen) return nerr_raise(NERR_ASSERT, "file too short");

            glPixelStorei(GL_UNPACK_ROW_LENGTH, y);
            glTexImage2D(GL_TEXTURE_2D, ix, li->internalFormat,
                         x, y, 0, li->externalFormat, li->type, data);

            x = (x+1) >> 1;
            y = (y+1) >> 1;
            size = x * y * li->blockBytes;
        }
        free(data);
        glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount-1);

    mtex_set_filtering_anisotropic(tnode->tex);

    glBindTexture(GL_TEXTURE_2D, 0);

    SAFE_FREE(buf);

    *a = (RendAsset*)tnode;

    return STATUS_OK;
}

void mast_dds_unload(void *p)
{
    /* TODO unload */
    if (!p) return;
}

NEOERR* mast_lut_load(char *dir, char *name, RendAsset **a)
{
    char fname[PATH_MAX], *buf;
    NEOERR *err;

    if (dir) snprintf(fname, sizeof(fname), "%s%s", dir, name);
    else strncpy(fname, name, sizeof(fname));

    TexAsset *tnode = mtex_node_new();
    if (!tnode) return nerr_raise(NERR_NOMEM, "alloc texture");

    err = ne_load_file(fname, &buf);
    if (err != STATUS_OK) return nerr_pass(err);

    int head = sizeof("CORANGE-LUT") - 1;
    int lut_size = (unsigned char)buf[head] | (unsigned char)buf[head+1];
    int offset = head + 3;

    glEnable(GL_TEXTURE_3D);
    glGenTextures(1, &tnode->tex);
    glBindTexture(GL_TEXTURE_3D, tnode->tex);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, lut_size, lut_size, lut_size,
                 0, GL_RGB, GL_UNSIGNED_BYTE, buf + offset);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
    glDisable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, 0);

    SAFE_FREE(buf);

    *a = (RendAsset*)tnode;

    return STATUS_OK;
}

void mast_lut_unload(void *p)
{
    if (!p) return;
}


TexAsset* mast_texture_new(GLuint texid)
{
    if (!glIsTexture(texid)) return NULL;

    TexAsset *tnode = mtex_node_new();
    tnode->tex = texid;

    return tnode;
}

NEOERR* mast_texture_load_raw(char *fname, GLuint *texid)
{
    GLuint rtex;
    int width, height;
    unsigned char *data;
    size_t len;
    FILE *file;

    MCS_NOT_NULLB(texid, fname);

    file = fopen(fname, "rb");
    if (!file) return nerr_raise(NERR_IO, "open %s failure", fname);

    width = 1024, height = 512; len = width * height *3;
    data = calloc(1, len);
    fread(data, len, 1, file);
    fclose(file);

    glGenTextures(1, &rtex);
    glBindTexture(GL_TEXTURE_2D, rtex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    free(data);

    MGL_CHECK_ERROR();

    *texid = rtex;

    return STATUS_OK;
}
