#ifndef __MENTITY_H__
#define __MENTITY_H__

#include "mheads.h"

__BEGIN_DECLS

#define ENTITY_KEY "_entities"
#define ENTITY_DRIVER_MAXNUM 100

enum {
    ENTITY_TYPE_STATIC = 0,
    ENTITY_TYPE_DYNAMIC,
    ENTITY_TYPE_CAMERA,
    ENTITY_TYPE_LIGHT
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
    RendAsset *shadowmat;
} RendEntity;

typedef struct {
    char *typename;
    int typeid;
    NEOERR* (*new)(HDF *enode, char *dir, RendEntity **e);
    NEOERR* (*onact)(RendEntity *ent, SDL_Event e, bool *running);
    NEOERR* (*update)(RendEntity *p, float dt);
    void (*free)(void *p);
} EntityDriver;

NEOERR* mentity_init();
void    mentity_finish();

NEOERR* mentity_driver_regist(EntityDriver *driver);

/*
 * dir: asset files's abs path for assembly entity, can be NULL
 */
NEOERR*     mentity_node_new(HDF *enode, char *dir, RendEntity **e);
void        mentity_node_free(void *p);
RendEntity* mentity_node_get(char *key);

/*
 * sdl event on entity, called per frame, per action, per node
 */
void        mentity_node_onact(RendEntity *ent, SDL_Event e, bool *running);
/*
 * update node entity, called per frame, per node
 */
void        mentity_node_update(RendEntity *e, float dt);

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
