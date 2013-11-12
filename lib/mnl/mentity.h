#ifndef __MENTITY_H__
#define __MENTITY_H__

#include "mheads.h"

__BEGIN_DECLS

#define ENTITY_KEY "_entities"
#define ENTITY_DRIVER_NUM 2

enum {
    ENTITY_TYPE_STATIC = 0,
    ENTITY_TYPE_CAMERA
};

typedef struct {
    char *name;
    int typeid;
    
    bool active;
    bool receive_shadows;
    bool cast_shadows;
    vec3 position;

    RendAsset *rendable;
    RendAsset *material;
} RendEntity;

typedef struct {
    char *name;
    NEOERR* (*new)(HDF *enode, char *dir, RendEntity **e);
    void (*free)(void *p);
} EntityDriver;

NEOERR* mentity_init();
void    mentity_finish();

/*
 * dir: asset files's abs path for assembly entity, can be NULL
 */
NEOERR*     mentity_node_new(HDF *enode, char *dir, RendEntity **e);
void        mentity_node_free(void *p);
RendEntity* mentity_node_get(char *key);

/*
 * new entity will be stored in g_datah
 * dir: is the entity config files's absolute path
 * assetdir: is the asset files's absolute path will be prepend
 * use absolute path in entity config file, and pass NULL as assetdir also ok
 */
NEOERR* mentity_load_file(char *dir, char *name, char *assetdir);
NEOERR* mentity_load_dir(char *dir, char *assetdir);


__END_DECLS
#endif    /* __MENTITY_H__ */
