#include "mheads.h"

static int mat_get_type(char *type)
{
    if (!type) return -1;

    if (!strcmp(type, "int")) return MAT_TYPE_INT;
    if (!strcmp(type, "float")) return MAT_TYPE_FLOAT;
    if (!strcmp(type, "vec2")) return MAT_TYPE_VEC2;
    if (!strcmp(type, "vec3")) return MAT_TYPE_VEC3;
    if (!strcmp(type, "vec4")) return MAT_TYPE_VEC4;
    if (!strcmp(type, "shader")) return MAT_TYPE_SHADER;
    if (!strcmp(type, "texture")) return MAT_TYPE_TEXTURE;

    return -1;
}

static MatItem* mat_item_new(char *dir, char *type, char *name, char *value)
{
    float f0, f1, f2, f3;
    char *end;
    MatItem *item;
    NEOERR *err;

    item = calloc(1, sizeof(MatItem));
    if (!item) return NULL;

    item->name = strdup(name);
    item->type = mat_get_type(type);
    
    switch (item->type) {
    case MAT_TYPE_INT:
        item->as.i = atoi(value);
        break;
    case MAT_TYPE_FLOAT:
        item->as.f = atof(value);
        break;
    case MAT_TYPE_VEC2:
        f0 = strtod(value, &end); f1 = strtod(end, NULL);
        item->as.v2 = vec2_new(f0, f1);
        break;
    case MAT_TYPE_VEC3:
        f0 = strtod(value, &end); f1 = strtod(end, &end);
        f2 = strtod(end, NULL);
        item->as.v3 = vec3_new(f0, f1, f2);
        break;
    case MAT_TYPE_VEC4:
        f0 = strtod(value, &end); f1 = strtod(end, &end);
        f2 = strtod(value, &end); f3 = strtod(end, NULL);
        item->as.v4 = vec4_new(f0, f1, f2, f3);
        break;
    case MAT_TYPE_SHADER:
    case MAT_TYPE_TEXTURE:
        err = masset_node_load(dir, value, &item->as.a);
        RETURN_V_NOK(err, NULL);
        break;
    default:
        mtc_dbg("unknown type %s", type);
        return NULL;
    }
    
    return item;
}

static void mat_item_free(void *p)
{
    SAFE_FREE(p);
}

static MatEntry* mat_entry_new()
{
    MatEntry *e = calloc(1, sizeof(MatEntry));
    if (!e) return NULL;

    uListInit(&e->items, 0, 0);

    return e;
}

static void mat_entry_free(void *p)
{
    if (!p) return;

    MatEntry *me = p;

    mast_shader_prog_free(me->prog);

    uListDestroyFunc(&me->items, mat_item_free);
}

static NEOERR* mat_generate_program(MatEntry *me)
{
    MatItem *item;
    
    MCS_NOT_NULLA(me);

    me->prog = mast_shader_prog_new();

    MLIST_ITERATE(me->items, item) {
        if (item->type == MAT_TYPE_SHADER) {
            mast_shader_prog_attach_shader(me->prog, item->as.a);
        }
    }

    mast_shader_prog_link(me->prog);

    return STATUS_OK;
}

static MatAsset* mat_node_new()
{
    MatAsset *mat = calloc(1, sizeof(MatAsset));
    if (!mat) return NULL;

    uListInit(&mat->entries, 0, 0);

    return mat;
}

NEOERR* mast_mat_load(char *dir, char *name, RendAsset **a)
{
    char fname[PATH_MAX], *buf, *line;
    ULIST *lines;
    NEOERR *err;
    
    if (dir) snprintf(fname, sizeof(fname), "%s%s", dir, name);
    else strncpy(fname, name, sizeof(fname));

    err = ne_load_file(fname, &buf);
    if (err != STATUS_OK) return nerr_pass(err);

    err = mstr_array_fast_split(&lines, buf, "\n", 1048576);
    if (err != STATUS_OK) return nerr_pass(err);

    MatAsset *mat = mat_node_new();
    if (!mat) return nerr_raise(NERR_NOMEM, "alloc mat");

    MatEntry *me = mat_entry_new();
    if (!me) return nerr_raise(NERR_NOMEM, "alloc entry");

    MLIST_ITERATE(lines, line) {
        line = neos_strip(line);

        if (line[0] == '#') continue;

        char type[512], name[512], value[512];
        int matches = sscanf(line, "%511s %511s = %511s", type, name, value);

        if (!strcmp(type, "submaterial")) {
            if ((mat->entries->num == 0) && (me->items->num == 0)) continue;

            err = mat_generate_program(me);
            CONTINUE_NOK(err);
            
            uListAppend(mat->entries, me);
            me = mat_entry_new();

            continue;
        }

        if (matches != 3) continue;

        MatItem *item = mat_item_new(dir, type, name, value);
        if (item) uListAppend(me->items, item);
    }

    if (me->items->num > 0) {
        err = mat_generate_program(me);
        uListAppend(mat->entries, me);
    }

    uListDestroy(&lines, 0);
    SAFE_FREE(buf);

    *a = (RendAsset*)mat;
    
    return STATUS_OK;
}

void mast_mat_unload(void *p)
{
    if (!p) return;

    MatAsset *a = p;

    uListDestroyFunc(&a->entries, mat_entry_free);
}
