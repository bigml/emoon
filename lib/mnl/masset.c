#include "mheads.h"

static ULIST *asset_drivers = NULL;

static int asset_driver_comp(const void *a, const void*b)
{
    if (a == NULL) return -1;
    if (b == NULL) return 1;
    
    char *key = (char*)a;
    AssetDriver *pb = *(AssetDriver**)b;

    return strcmp(key, pb->name);
}

static void hash_asset_free(void *a)
{
    HASHNODE *node = a;

    if (node) {
        free(node->key);
        masset_node_unload(node->value);
    }
}

static AssetDriver* masset_driver_new(char *ext,
                                      NEOERR* load(char *dir, char *name, RendAsset **a),
                                      void unload(void *p))
{
    AssetDriver *d = calloc(1, sizeof(AssetDriver));
    if (!d) return NULL;

    d->name = strdup(ext);
    d->load = load;
    d->unload = unload;

    return d;
}

static void masset_driver_free(void *p)
{
    if (!p) return;

    AssetDriver *a = p;

    SAFE_FREE(a->name);
    free(a);
}

NEOERR* masset_init()
{
    NEOERR *err;
    
    if (!g_datah) {
        err = hash_init(&g_datah, hash_str_hash, hash_str_comp, NULL);
        if (err != STATUS_OK) return nerr_pass(err);
    }

    HASH *mh = hash_lookup(g_datah, ASSET_KEY);
    if (!mh) {
        err = hash_init(&mh, hash_str_hash, hash_str_comp, hash_asset_free);
        if (err != STATUS_OK) return nerr_pass(err);

        hash_insert(g_datah, ASSET_KEY, (void*)mh);
    }

    if (asset_drivers) return STATUS_OK;
    
    err = uListInit(&asset_drivers, 10, 0);
    if (err != STATUS_OK) return nerr_pass(err);
    
    uListAppend(asset_drivers, masset_driver_new("obj", mast_vb_obj_load, mast_vb_unload));
    uListAppend(asset_drivers, masset_driver_new("mat", mast_mat_load, mast_mat_unload));
    
    uListAppend(asset_drivers, masset_driver_new("vs", mast_vs_load, mast_shader_unload));
    uListAppend(asset_drivers, masset_driver_new("fs", mast_fs_load, mast_shader_unload));
    uListAppend(asset_drivers, masset_driver_new("dds", mast_dds_load, mast_dds_unload));

    return STATUS_OK;
}

void masset_finish()
{
    HASH *mh = hash_lookup(g_datah, ASSET_KEY);
    hash_destroy(&mh);
    hash_remove(g_datah, ASSET_KEY);
    
    uListDestroyFunc(&asset_drivers, masset_driver_free);

    asset_drivers = NULL;
}
        
NEOERR* masset_node_load(char *dir, char *name, RendAsset **pa)
{       
    AssetDriver *driver;
    RendAsset *asset = NULL;
    int driverindex;
    HASH *mh;
    NEOERR *err;

    mh = hash_lookup(g_datah, ASSET_KEY);
        
    MCS_NOT_NULLC(name, pa, mh);

    *pa = NULL;
        
    char *p = strrchr(name, '.');
    if (p) p = p + 1;
    
    if (!p || !*p) return nerr_raise(NERR_ASSERT, "unknown asset type %s", name);

    driverindex = uListIndex(asset_drivers, p, asset_driver_comp);
    if (driverindex < 0) return nerr_raise(NERR_ASSERT, "unknown asset type %s", name);
    
    err = uListGet(asset_drivers, driverindex, (void**)&driver);
    if (err != STATUS_OK) return nerr_pass(err);

    mtc_dbg("load asset %s%s use %s %d", dir, name, driver->name, driverindex);
    
    err = driver->load(dir, name, &asset);
    if (err != STATUS_OK) return nerr_pass(err);
    if (!asset) return nerr_raise(NERR_ASSERT, "asset node %s empty", name);

    asset->name = strdup(name);
    asset->driverindex = driverindex;

    hash_remove(mh, name);
    hash_insert(mh, strdup(name), asset);

    *pa = asset;

    return STATUS_OK;
}

void masset_node_unload(void *p)
{
    NEOERR *err;
    
    if (!p) return;

    RendAsset *a = p;
    AssetDriver *driver;

    err = uListGet(asset_drivers, a->driverindex, (void**)&driver);
    RETURN_NOK(err);

    mtc_dbg("unload asset %s use %s %d", a->name, driver->name, a->driverindex);
    
    if (driver) driver->unload(a);

    SAFE_FREE(a->name);
    SAFE_FREE(a);
    
    /* TODO remove from asset hash table? */
}
