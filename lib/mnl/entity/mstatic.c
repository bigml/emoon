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
    char *ename, *etype, *ren, *mat, *pos, *smat;
    NEOERR *err;

    MCS_NOT_NULLB(enode, pe);

    *pe = NULL;

    ename = hdf_obj_name(enode);
    etype = mcs_obj_attr(enode, "type");
    ren = hdf_get_value(enode, "rendable", NULL);
    mat = hdf_get_value(enode, "material", NULL);
    smat = hdf_get_value(enode, "shadow_material", NULL);
    pos = hdf_get_value(enode, "position", "0 0 0");

    if (etype && strcmp(etype, "static"))
        return nerr_raise(NERR_ASSERT, "type %s not match", etype);

    StaticEntity *e = calloc(1, sizeof(StaticEntity));
    if (!e) return nerr_raise(NERR_NOMEM, "alloc entity");

    e->base.name = strdup(ename);
    e->base.typeid = ENTITY_TYPE_STATIC;
    e->base.active = true;
    e->base.receive_shadows = true;
    e->base.cast_shadows = true;
    e->base.position = vec3_from_string(pos);
    e->base.shadowmat = NULL;

    err = masset_node_load(dir, ren, &e->base.rendable);
    if (err != STATUS_OK) goto errorexit;

    err = masset_node_load(dir, mat, &e->base.material);
    if (err != STATUS_OK) goto errorexit;

    if (smat) {
        err = masset_node_load(dir, smat, &e->base.shadowmat);
        if (err != STATUS_OK) goto errorexit;
    }

    e->rotation = vec4_from_string(hdf_get_value(enode, "rotation", "0 0 0 1"));
    e->scale = vec3_from_string(hdf_get_value(enode, "scale", "1 1 1"));

    e->t = g_time_upms;

    e->phy.radius = 0.0;
    e->phy.m = mcs_get_float_value(enode, "weight", 0);
    e->phy.ffric = mcs_get_float_value(enode, "f_fric", 0);
    e->phy.fpull = mcs_get_float_value(enode, "f_pull", 0);
    e->phy.f = vec3_zero();
    e->phy.fmax = vec3_from_string(hdf_get_value(enode, "f_max", "0 0 0"));
    e->phy.v = vec3_from_string(hdf_get_value(enode, "velocity", "0 0 0"));
    e->phy.a = vec3_from_string(hdf_get_value(enode, "acceleration", "0 0 0"));

    e->evt.engine_on = false;
    e->evt.f = vec3_zero();

    MatAsset *m = (MatAsset*)e->base.material;
    MatEntry *me;
    for (int i = 0; i < m->entries->num; i++) {
        uListGet(m->entries, i, (void**)&me);
        if (e->base.receive_shadows) mast_mat_add_item_int(me, "receive_shadows", 1);
        else mast_mat_add_item_int(me, "receive_shadows", 0);
    }

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
