#ifndef __MASSET_H__
#define __MASSET_H__

#include "mheads.h"

__BEGIN_DECLS

#define ASSET_KEY "_assets"

typedef struct {
    char *name;
    int driverindex;
} RendAsset;

typedef struct {
    char *name;
    NEOERR* (*load)(char *dir, char *name, RendAsset **a);
    void (*unload)(void *p);
} AssetDriver;

NEOERR* masset_init();
void    masset_finish();

/*
 * new asset will be stored in g_datah, and return to caller
 */
NEOERR* masset_node_load(char *dir, char *name, RendAsset **pa);
void    masset_node_unload(void *p);

__END_DECLS
#endif    /* __MASSET_H__ */
