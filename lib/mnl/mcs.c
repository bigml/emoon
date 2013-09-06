#include "mheads.h"

HDF* mcs_obj_nth_child(HDF *hdf, int n)
{
    HDF *node;

    if (!hdf || n < 0) return NULL;

    node = hdf_obj_child(hdf);
    while (node && --n > 0) {
        node = hdf_obj_next(node);
    }

    return node;
}

HDF* mcs_get_objf(HDF *hdf, char *fmt, ...)
{
    char key[LEN_HDF_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return hdf_get_obj(hdf, key);
}

NEOERR* mcs_get_nodef(HDF *hdf, HDF **rnode, char *fmt, ...)
{
    char key[LEN_HDF_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return hdf_get_node(hdf, key, rnode);
}

NEOERR* mcs_copyf(HDF *dst, HDF *src, char *fmt, ...)
{
    char key[LEN_HDF_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return nerr_pass(hdf_copy(dst, key, src));
}

NEOERR* mcs_remove_treef(HDF *hdf, char *fmt, ...)
{
    char key[LEN_HDF_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return nerr_pass(hdf_remove_tree(hdf, key));
}

int  mcs_get_child_num(HDF *hdf, char *name)
{
    HDF *node;
    
    if (!hdf) return 0;

    int count = 0;

    node = hdf_get_child(hdf, name);
    while (node) {
        count++;
        node = hdf_obj_next(node);
    }

    return count;
}
int mcs_get_child_numf(HDF *hdf, char *fmt, ...)
{
    char key[LEN_HDF_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return mcs_get_child_num(hdf, key);
}

HDF* mcs_get_nth_child(HDF *hdf, char *name, int n)
{
    HDF *node;
    
    if (!hdf || n <= 0) return NULL;

    node = hdf_get_child(hdf, name);
    while (node && --n > 0) {
        node = hdf_obj_next(node);
    }

    return node;
}
HDF* mcs_get_nth_childf(HDF *hdf, int n, char *fmt, ...)
{
    char key[LEN_HDF_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return mcs_get_nth_child(hdf, key, n);
}


unsigned int mcs_get_uint_value(HDF *hdf, char *name, unsigned int defval)
{
    char *val, *n;
    unsigned int v;
    
    val = hdf_get_value(hdf, name, NULL);
    if (val) {
        v = strtoul(val, &n, 10);
        if (val == n) v = defval;
        return v;
    }
    return defval;
}

NEOERR* mcs_set_uint_value(HDF *hdf, char *name, unsigned int value)
{
    char buf[256];

    snprintf(buf, sizeof(buf), "%u", value);
    return nerr_pass(hdf_set_value(hdf, name, buf));
}

float mcs_get_float_value(HDF *hdf, char *name, float defval)
{
    char *val, *n;
    float v;
    
    val = hdf_get_value(hdf, name, NULL);
    if (val) {
        v = strtof(val, &n);
        if (val == n) v = defval;
        return v;
    }
    return defval;
}

NEOERR* mcs_set_float_value(HDF *hdf, char *name, float value)
{
    char buf[256];

    snprintf(buf, sizeof(buf), "%f", value);
    return nerr_pass(hdf_set_value(hdf, name, buf));
}

int64_t mcs_get_int64_value(HDF *hdf, char *name, int64_t defval)
{
    char *val, *n;
    int64_t v;

    val = hdf_get_value(hdf, name, NULL);
    if (val) {
        v = strtoll(val, &n, 10);
        if (val == n) v = defval;
        return v;
    }
    return defval;
}

NEOERR* mcs_set_int64_value(HDF *hdf, char *name, int64_t val)
{
    char buf[256];
    
    snprintf(buf, sizeof(buf), "%ld", val);
    return nerr_pass(hdf_set_value(hdf, name, buf));
}

int mcs_get_int_valuef(HDF *hdf, int defval, char *fmt, ...)
{
    char key[LEN_HDF_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return hdf_get_int_value(hdf, key, defval);
}

int mcs_add_int_value(HDF *node, char *key, int val)
{
    if (!node || !key) return 0;
    
    int ov = hdf_get_int_value(node, key, 0);
    hdf_set_int_value(node, key, ov+val);

    return ov+val;
}

int mcs_add_int_valuef(HDF *node, int val, char *fmt, ...)
{
    char key[LEN_HDF_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return mcs_add_int_value(node, key, val);
}

int64_t mcs_add_int64_value(HDF *node, char *key, int64_t val)
{
    if (!node || !key) return 0;
    
    int64_t ov = mcs_get_int64_value(node, key, 0);
    mcs_set_int64_value(node, key, ov+val);

    return ov+val;
}

int64_t mcs_add_int64_valuef(HDF *node, int64_t val, char *fmt, ...)
{
    char key[LEN_HDF_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return mcs_add_int64_value(node, key, val);
}

char* mcs_append_string_value(HDF *node, char *key, char *str)
{
    if (!node || !key) return NULL;

    char *ov = hdf_get_value(node, key, "");

    hdf_set_valuef(node, "%s=%s%s", key, ov, str);

    return hdf_get_value(node, key, NULL);
}

char* mcs_append_string_valuef(HDF *node, char *key, char *sfmt, ...)
{
    char *qa, *rs;
    va_list ap;
    
    va_start(ap, sfmt);
    qa = vsprintf_alloc(sfmt, ap);
    va_end(ap);
    if (!qa) return NULL;

    rs = mcs_append_string_value(node, key, qa);
    
    free(qa);

    return rs;
}

char* mcs_prepend_string_value(HDF *node, char *key, char *str)
{
    if (!node || !key) return NULL;

    char *ov = hdf_get_value(node, key, "");

    hdf_set_valuef(node, "%s=%s%s", key, str, ov);

    return hdf_get_value(node, key, NULL);
}

char* mcs_prepend_string_valuef(HDF *node, char *key, char *sfmt, ...)
{
    char *qa, *rs;
    va_list ap;
    
    va_start(ap, sfmt);
    qa = vsprintf_alloc(sfmt, ap);
    va_end(ap);
    if (!qa) return NULL;

    rs = mcs_prepend_string_value(node, key, qa);
    
    free(qa);

    return rs;
}


char* mcs_hdf_attr(HDF *hdf, char *name, char*key)
{
    if (hdf == NULL || key == NULL)
        return NULL;
    
    HDF_ATTR *attr = hdf_get_attr(hdf, name);
    while (attr != NULL) {
        if (!strcmp(attr->key, key)) {
            return attr->value;
        }
        attr = attr->next;
    }
    return NULL;
}

char* mcs_obj_attr(HDF *hdf, char*key)
{
    if (hdf == NULL || key == NULL)
        return NULL;
    
    HDF_ATTR *attr = hdf_obj_attr(hdf);
    while (attr != NULL) {
        if (!strcmp(attr->key, key)) {
            return attr->value;
        }
        attr = attr->next;
    }
    return NULL;
}

int mcs_get_int_attr(HDF *hdf, char *name, char *key, int defval)
{
    char *s = mcs_hdf_attr(hdf, name, key);

    if (s) return atoi(s);
    else return defval;
}

NEOERR* mcs_set_int_attr(HDF *hdf, char *name, char *key, int val)
{
    char tok[64] = {0};
    
    snprintf(tok, sizeof(tok), "%d", val);

    return nerr_pass(hdf_set_attr(hdf, name, key, tok));
}

NEOERR* mcs_set_int_attrr(HDF *hdf, char *name, char *key, int val)
{
    char tok[64] = {0};
    
    snprintf(tok, sizeof(tok), "%d", val);

    /* can't set node's attr if node have no value */
    if (!hdf_get_value(hdf, name, NULL)) hdf_set_value(hdf, name, NULL);

    return nerr_pass(hdf_set_attr(hdf, name, key, tok));
}

NEOERR* mcs_set_value_with_type(HDF *hdf, char *name, char *value,
                                CnodeType type)
{
    NEOERR *err;

    err = hdf_set_value(hdf, name, value);
    if (err != STATUS_OK) return nerr_pass(err);

    return nerr_pass(mcs_set_int_attr(hdf, name, "type", type));
}

NEOERR* mcs_set_valuef_with_type(HDF *hdf, CnodeType type, char *fmt, ...)
{
    char *k, *v;
    va_list ap;
    NEOERR *err;

    va_start(ap, fmt);
    k = vsprintf_alloc(fmt, ap);
    if (!k) return nerr_raise(NERR_NOMEM, "alloc format string");
    va_end(ap);

    v = strchr(k, '=');
    if (!v) return nerr_raise(NERR_ASSERT, "No equials found: %s", k);
    *v++ = '\0';                /* equal to *v = '\0'; v++; */

    err = mcs_set_value_with_type(hdf, k, v, type);

    SAFE_FREE(k);

    return nerr_pass(err);
}


void mcs_hdf_rep(HDF *data, HDF *dst)
{
    char *srcstr, *repstr;
    
    if (!data || !dst) return;

    HDF *datarow = hdf_obj_child(data);
    while (datarow) {
        HDF *child = hdf_obj_child(dst);
        while (child) {
            if (hdf_obj_child(child)) {
                return mcs_hdf_rep(data, child);
            }

            srcstr = hdf_obj_value(child);
            repstr = mstr_repstr(1, srcstr,
                                 hdf_obj_name(datarow),
                                 hdf_obj_value(datarow));
            hdf_set_value(child, NULL, repstr);
            free(repstr);

            child = hdf_obj_next(child);
        }
        
        datarow = hdf_obj_next(datarow);
    }
}

NEOERR* mcs_hdf_copy_rep(HDF *dst, char *name, HDF *src, HDF *data)
{
    NEOERR *err;
    
    MCS_NOT_NULLB(dst, src);

    err = hdf_copy(dst, name, src);
	if (err != STATUS_OK) return nerr_pass(err);

    mcs_hdf_rep(data, hdf_get_obj(dst, name));

    return STATUS_OK;
}

char* mcs_repvstr_byhdf(char *src, char c, HDF *data)
{
    char *p, key[LEN_HDF_KEY], *val;
    int x;
    STRING str;
    
    string_init(&str);

    if (!src) return NULL;
    if (!data) return strdup(src);

    p = src;
    
    while (*p) {
        if (*p != c) {
            string_append_char(&str, *p);
            p++;
        } else if (*p == c) {
            memset(key, 0x0, sizeof(key));
            x = 0;

            /*
             * skip series start $
             */
            while (*p && *p == c) p++;
            
            while (*p && *p != c && x < LEN_HDF_KEY) {
                key[x++] = *p;
                p++;
            }

            if (x > 0) {
                /*
                 * skip single end $
                 */
                if (*p == c) p++;
                
                val = hdf_get_value(data, key, NULL);
                if (val) string_append(&str, val);
            }
        }
    }

    return str.buf;
}


NEOERR* mcs_str2file(STRING str, const char *file)
{
    if (file == NULL) return nerr_raise(NERR_ASSERT, "paramter null");
    
    FILE *fp = fopen(file, "w");
    if (!fp) return nerr_raise(NERR_IO, "unable to open %s for write", file);

    size_t ret = fwrite(str.buf, str.len, 1, fp);
    if (ret < 0) {
        fclose(fp);
        return nerr_raise(NERR_IO, "write str.buf to %s error", file);
    }
    
    fclose(fp);
    return STATUS_OK;
}

void* hash_lookupf(HASH *table, char *fmt, ...)
{
    char key[LEN_HASH_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return hash_lookup(table, key);
}

NEOERR* hash_insertf(HASH *table, void *data, char *fmt, ...)
{
    char key[LEN_HASH_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return hash_insert(table, strdup(key), data);
}

NEOERR* mcs_err_valid(NEOERR *err)
{
    NEOERR *r = err;

    if (err == STATUS_OK) return STATUS_OK;

    while (r && r != INTERNAL_ERR) {
        if (r->error != NERR_PASS) break;
        r = r->next;
    }

    if (!r) r = err;
    
    return r;
}
