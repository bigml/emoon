#ifndef __MMACRO_H__
#define __MMACRO_H__

#include "mheads.h"

/*
 * clearsilver related macros
 */

#define MCS_NOT_NULLA(pa)                                       \
    if (!pa) return nerr_raise(NERR_ASSERT, "paramter null");
#define MCS_NOT_NULLB(pa, pb)                                       \
    do {                                                            \
        if (!pa) return nerr_raise(NERR_ASSERT, "paramter a null"); \
        if (!pb) return nerr_raise(NERR_ASSERT, "paramter b null"); \
    } while (0)
#define MCS_NOT_NULLC(pa, pb, pc)                                   \
    do {                                                            \
        if (!pa) return nerr_raise(NERR_ASSERT, "paramter a null"); \
        if (!pb) return nerr_raise(NERR_ASSERT, "paramter b null"); \
        if (!pc) return nerr_raise(NERR_ASSERT, "paramter c null"); \
    } while (0)

#define ITERATE_MLIST(ul)                               \
    for (int t_rsv_i = 0; t_rsv_i < ul->num; t_rsv_i++)


#define DIE_NOK(err)                            \
    if (err != STATUS_OK) {                     \
        STRING zstra;    string_init(&zstra);   \
        nerr_error_traceback(err, &zstra);      \
        mtc_err("%s", zstra.buf);               \
        string_clear(&zstra);                   \
        nerr_ignore(&err);                      \
        exit(-1);                               \
    }

#define JUMP_NOK(err, label)                    \
    if (err != STATUS_OK) {                     \
        STRING zstra;    string_init(&zstra);   \
        nerr_error_traceback(err, &zstra);      \
        mtc_err("%s", zstra.buf);               \
        string_clear(&zstra);                   \
        nerr_ignore(&err);                      \
        goto label;                             \
    }

#define CONTINUE_NOK(err)                       \
    if (err != STATUS_OK) {                     \
        STRING zstra;    string_init(&zstra);   \
        nerr_error_traceback(err, &zstra);      \
        mtc_err("%s", zstra.buf);               \
        string_clear(&zstra);                   \
        nerr_ignore(&err);                      \
        continue;                               \
    }

#define RETURN_NOK(err)                         \
    if (err != STATUS_OK) {                     \
        STRING zstra;    string_init(&zstra);   \
        nerr_error_traceback(err, &zstra);      \
        mtc_err("%s", zstra.buf);               \
        string_clear(&zstra);                   \
        nerr_ignore(&err);                      \
        return;                                 \
    }

#define RETURN_V_NOK(err, v)                    \
    if (err != STATUS_OK) {                     \
        STRING zstra;    string_init(&zstra);   \
        nerr_error_traceback(err, &zstra);      \
        mtc_err("%s", zstra.buf);               \
        string_clear(&zstra);                   \
        nerr_ignore(&err);                      \
        return v;                               \
    }

#define TRACE_NOK(err)                          \
    if (err != STATUS_OK) {                     \
        STRING zstra;    string_init(&zstra);   \
        nerr_error_traceback(err, &zstra);      \
        mtc_err("%s", zstra.buf);               \
        string_clear(&zstra);                   \
        nerr_ignore(&err);                      \
    }

#define OUTPUT_NOK(err)                         \
    if (err != STATUS_OK) {                     \
        STRING zstra;    string_init(&zstra);   \
        nerr_error_traceback(err, &zstra);      \
        printf("%s", zstra.buf);                \
        string_clear(&zstra);                   \
        nerr_ignore(&err);                      \
    }

/*
 * Output {
 *     _npp = 15
 *     _npg = 3
 *     points [type=107]
 * }
 * won't care(trace) points node, which have no value.
 * but hdf_write_string(), mjson_import_from_hdf(), mbson_import_from_hdf() will
 */
#define TRACE_HDF(node)                         \
    do {                                        \
        STRING zstra;    string_init(&zstra);   \
        hdf_dump_str(node, NULL, 2, &zstra);    \
        mtc_foo("%s", zstra.buf);               \
        string_clear(&zstra);                   \
    } while (0)

/*
 * a set of macros for performance purpose. see mjson_export_to_hdf()
 */
#define MCS_SET_INT_ATTR(hdf, name, key, val)   \
    do {                                        \
        char ztoka[64];                         \
        snprintf(ztoka, 64, "%d", val);         \
        hdf_set_attr(hdf, name, key, ztoka);    \
    } while (0)
#define MCS_SET_INT_ATTRR(hdf, name, key, val)  \
    do {                                        \
        char ztoka[64];                         \
        snprintf(ztoka, 64, "%d", val);         \
        if (!hdf_get_value(hdf, name, NULL))    \
            hdf_set_value(hdf, name, "foo");    \
        hdf_set_attr(hdf, name, key, ztoka);    \
    } while (0)
#define MCS_GET_INT_ATTR(hdf, name, key, defval, v) \
    do {                                            \
        char *zsa = mcs_hdf_attr(hdf, name, key);   \
        if (zsa) v = atoi(zsa);                     \
        else v = defval;                            \
    } while (0)

/*
 * OpenGL related 
 */
#define MGL_CHECK_ERROR()                                       \
    do {                                                        \
        GLenum _glerror = glGetError();                         \
        if (_glerror) {                                         \
            mtc_err("OpenGL Error: %s", mgl_errstr(_glerror));  \
        }                                                       \
    } while (0)

#define MGL_CHECK_FRAMEBUFFERSTATUS()                                   \
    do {                                                                \
        GLenum _glerror = glCheckFramebufferStatus(GL_FRAMEBUFFER);     \
        if (_glerror != GL_FRAMEBUFFER_COMPLETE) {                      \
            mtc_err("Framebuffer Error: %s", mgl_errstr_framebuffer(_glerror)); \
        }                                                               \
    } while (0)

/*
 * other stuff
 */
#define SAFE_FREE(str)                          \
    do {                                        \
        if (str != NULL) {                      \
            free(str);                          \
            str = NULL;                         \
        }                                       \
    } while (0)

#endif  /* __MMACRO_H__ */
