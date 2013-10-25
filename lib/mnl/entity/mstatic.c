#include "mheads.h"

static void mstatic_free(void *p)
{
    StaticEntity *e = p;

    if (!e) return;
    
    free(e->base.name);
    free(e);
}

static NEOERR* mstatic_new(HDF *enode, char *dir, RendEntity **pe)
{
    char *ename, *etype, *ren, *mat;
    NEOERR *err;

    MCS_NOT_NULLB(enode, pe);

    *pe = NULL;
    
    ename = hdf_obj_name(enode);
    etype = mcs_obj_attr(enode, "type");
    ren = hdf_get_value(enode, "rendable", NULL);
    mat = hdf_get_value(enode, "material", NULL);

    if (etype && strcmp(etype, "static"))
        return nerr_raise(NERR_ASSERT, "type %s not match", etype);

    StaticEntity *e = calloc(1, sizeof(StaticEntity));
    if (!e) return nerr_raise(NERR_NOMEM, "alloc entity");
    
    e->base.name = strdup(ename);
    e->base.typeid = ENTITY_TYPE_STATIC;
    e->base.active = true;
    e->base.receive_shadows = true;
    e->base.cast_shadows = true;

    err = masset_node_load(dir, ren, &e->base.rendable);
    if (err != STATUS_OK) goto errorexit;
    
    err = masset_node_load(dir, mat, &e->base.material);
    if (err != STATUS_OK) goto errorexit;
    
    e->position = vec3_zero();
    e->rotation = quaternion_id();
    e->scale = vec3_one();

    *pe = (RendEntity*)e;
    
    return STATUS_OK;

errorexit:
    mstatic_free(e);
    return nerr_pass(err);
}

EntityDriver static_entity_driver = {
    .name = "static",
    .new = mstatic_new,
    .free = mstatic_free,
};
