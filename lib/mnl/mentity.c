#include "mheads.h"

static EntityDriver *entity_drivers[ENTITY_DRIVER_NUM] = {
    &static_entity_driver,
};

#ifdef __MACH__
static int mentity_config(struct dirent *ent)
#else
static int mentity_config(const struct dirent *ent)
#endif
{
    if (reg_search(".*.hdf", ent->d_name))
        return 1;
    else
        return 0;
}

static int mentity_type(char *type)
{
    if (!type) return -1;
    
    if (!strcmp(type, "static")) return 0;

    return -1;
}

static void hash_entity_free(void *a)
{
    HASHNODE *node = a;

    if (node) {
        free(node->key);
        mentity_node_free(node->value);
    }
}

NEOERR* mentity_init()
{
    NEOERR *err;
    
    if (!g_datah) {
        err = hash_init(&g_datah, hash_str_hash, hash_str_comp, NULL);
        if (err != STATUS_OK) return nerr_pass(err);
    }

    HASH *eh = hash_lookup(g_datah, ENTITY_KEY);
    if (!eh) {
        err = hash_init(&eh, hash_str_hash, hash_str_comp, hash_entity_free);
        if (err != STATUS_OK) return nerr_pass(err);

        hash_insert(g_datah, ENTITY_KEY, (void*)eh);
    }
    
    return STATUS_OK;
}

void mentity_finish()
{
    HASH *eh = hash_lookup(g_datah, ENTITY_KEY);
    hash_destroy(&eh);
    hash_remove(g_datah, ENTITY_KEY);
}

NEOERR* mentity_node_new(HDF *enode, char *dir, RendEntity **e)
{
    char *ename, *etype;
    HASH *eh;
    NEOERR *err;

    eh = hash_lookup(g_datah, ENTITY_KEY);
    
    MCS_NOT_NULLC(enode, e, eh);

    ename = hdf_obj_name(enode);
    etype = mcs_obj_attr(enode, "type");
    if (!etype) etype = "static";

    int typeid = mentity_type(etype);
    if (typeid < 0) return nerr_raise(NERR_ASSERT, "unkown type %s", etype);

    err = entity_drivers[typeid]->new(enode, dir, e);
    if (err != STATUS_OK) return nerr_pass(err);
    
    hash_remove(eh, ename);
    hash_insert(eh, strdup(ename), *e);

    return STATUS_OK;
}

void mentity_node_free(void *p)
{
    if (!p) return;
    
    RendEntity *e = p;
    entity_drivers[e->typeid]->free(e);
}

NEOERR* mentity_load_file(char *dir, char *name, char *assetdir)
{
    char fname[PATH_MAX];
    HDF *node, *cnode;
    NEOERR *err;

    MCS_NOT_NULLB(dir, name);

    snprintf(fname, sizeof(fname), "%s/%s", dir, name);
    
    err = hdf_init(&node);
    if (err != STATUS_OK) return nerr_pass(err);
    
    err = hdf_read_file(node, fname);
    if (err != STATUS_OK) return nerr_pass(err);

    RendEntity *e;
    char *ename;
    cnode = hdf_obj_child(node);
    while (cnode) {
        ename = hdf_obj_name(cnode);
        mtc_dbg("new entity %s", ename);

        err = mentity_node_new(cnode, assetdir, &e);
        JUMP_NOK(err, wnext);

    wnext:
        cnode = hdf_obj_next(cnode);
    }
    
    return STATUS_OK;
}

NEOERR* mentity_load_dir(char *dir, char *assetdir)
{
    struct dirent **eps = NULL;
    int n;
    NEOERR *err;
    
    MCS_NOT_NULLA(dir);

    n = scandir(dir, &eps, mentity_config, alphasort);
    for (int i = 0; i < n; i++) {
        mtc_dbg("load entity config %s", eps[i]->d_name);

        err = mentity_load_file(dir, eps[i]->d_name, assetdir);
        TRACE_NOK(err);
        free(eps[i]);
    }

    if (n > 0) free(eps);
    else mtc_warn("no .hdf file found in %s", dir);
    
    return STATUS_OK;
}
