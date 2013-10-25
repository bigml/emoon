#ifndef __MCS_H__
#define __MCS_H__

/*
 * clearsilver related wrap
 */

#include "mheads.h"

__BEGIN_DECLS

/*
 * when you have defined item
 */
#define MLIST_ITERATE(list, item)                                       \
    item = list->items[0];                                              \
    for (int t_rsv_i = 0; t_rsv_i < list->num; item = list->items[++t_rsv_i])

/*
 * when you don't offer item
 */
#define ITERATE_MLIST(ul)                               \
    for (int t_rsv_i = 0; t_rsv_i < ul->num; t_rsv_i++)

/* n = 1 for 1st child */
HDF*    mcs_obj_nth_child(HDF *hdf, int n);

HDF*    mcs_get_objf(HDF *hdf, char *fmt, ...)
                     ATTRIBUTE_PRINTF(2, 3);
NEOERR* mcs_get_nodef(HDF *hdf, HDF **rnode, char *fmt, ...)
                      ATTRIBUTE_PRINTF(3, 4);
NEOERR* mcs_copyf(HDF *dst, HDF *src, char *fmt, ...)
                  ATTRIBUTE_PRINTF(3, 4);
NEOERR* mcs_remove_treef(HDF *hdf, char *fmt, ...)
                         ATTRIBUTE_PRINTF(2, 3);
int     mcs_get_child_num(HDF *hdf, char *name);
int     mcs_get_child_numf(HDF *hdf, char *fmt, ...)
                           ATTRIBUTE_PRINTF(2, 3);
HDF*    mcs_get_nth_child(HDF *hdf, char *name, int n);
HDF*    mcs_get_nth_childf(HDF *hdf, int n, char *fmt, ...)
                           ATTRIBUTE_PRINTF(3, 4);


unsigned int mcs_get_uint_value(HDF *hdf, char *name, unsigned int defval);
NEOERR*      mcs_set_uint_value(HDF *hdf, char *name, unsigned int value);

float        mcs_get_float_value(HDF *hdf, char *name, float defval);
NEOERR*      mcs_set_float_value(HDF *hdf, char *name, float value);

int64_t      mcs_get_int64_value(HDF *hdf, char *name, int64_t defval);
NEOERR*      mcs_set_int64_value(HDF *hdf, char *name, int64_t val);

int          mcs_get_int_valuef(HDF *hdf, int defval, char *fmt, ...)
                                ATTRIBUTE_PRINTF(3, 4);

int     mcs_add_int_value(HDF *node, char *key, int val);
int     mcs_add_int_valuef(HDF *node, int val, char *fmt, ...)
                           ATTRIBUTE_PRINTF(3, 4);
int64_t mcs_add_int64_value(HDF *node, char *key, int64_t val);
int64_t mcs_add_int64_valuef(HDF *node, int64_t val, char *fmt, ...)
                             ATTRIBUTE_PRINTF(3, 4);
char*   mcs_append_string_value(HDF *node, char *key, char *str);
char*   mcs_append_string_valuef(HDF *node, char *key, char *sfmt, ...)
                                 ATTRIBUTE_PRINTF(3, 4);
char*   mcs_prepend_string_value(HDF *node, char *key, char *str);
char*   mcs_prepend_string_valuef(HDF *node, char *key, char *sfmt, ...)
                                  ATTRIBUTE_PRINTF(3, 4);


char*   mcs_hdf_attr(HDF *hdf, char *name, char*key);
char*   mcs_obj_attr(HDF *hdf, char*key);
int     mcs_get_int_attr(HDF *hdf, char *name, char *key, int defval);
NEOERR* mcs_set_int_attr(HDF *hdf, char *name, char *key, int val);
/* set name = NULL if !hdf_get_value(hdf, name, NULL), and then set attr */
NEOERR* mcs_set_int_attrr(HDF *hdf, char *name, char *key, int val);

NEOERR* mcs_set_value_with_type(HDF *hdf, char *name, char *value, CnodeType type);
NEOERR* mcs_set_valuef_with_type(HDF *hdf, CnodeType type, char *fmt, ...);


/*
 * in:
 *
 * data {
 *    NeedReplaceA = foo
 *    NeedReplaceB = bar
 * }
 * dst {
 *    class = senior
 *    comment {
 *        nick = you have a NeedReplaceA comment
 *    }
 *    remark = when you are in NeedReplaceB...
 * }
 *
 * out:
 *
 * dst {
 *    class = senior
 *    comment {
 *        nick = you have a foo comment
 *    }
 *    remark = when you are in bar...
 * }
 */
void    mcs_hdf_rep(HDF *data, HDF *dst);
/* copy src hdf to dst.name hdf, and replace dst.name use data hdf */
NEOERR* mcs_hdf_copy_rep(HDF *dst, char *name, HDF *src, HDF *data);

/*
 * in:
 *
 * src = $level$, $level$$name$,  name.$level$.in$desc$,  or name.$level$
 * data {
 *     level = 100
 *     name = test data
 *     desc = a desc
 * }
 * c = $
 *
 * out
 *
 * 100, 100test data, name.100.ina desc, or name.100
 */
char* mcs_repvstr_byhdf(char *src, char c, HDF *data);


NEOERR* mcs_str2file(STRING str, const char *file);
void*   hash_lookupf(HASH *table, char *fmt, ...)
                     ATTRIBUTE_PRINTF(2, 3);
NEOERR* hash_insertf(HASH *table, void *data, char *fmt, ...)
                     ATTRIBUTE_PRINTF(3, 4);
/* skip NERR_PASS to found the first acture error */
NEOERR* mcs_err_valid(NEOERR *err);

__END_DECLS
#endif    /* __MCS_H__ */
