#include "mheads.h"

void mentity_dynamic_free(void *p)
{
    DynamicEntity *e = p;

    if (!e) return;

    free(e->base.name);
    free(e);
}

NEOERR* mentity_dynamic_new(HDF *enode, char *dir, RendEntity **pe)
{
    char *ename, *ren, *mat, *pos, *smat;
    NEOERR *err;

    MCS_NOT_NULLB(enode, pe);

    *pe = NULL;

    ename = hdf_obj_name(enode);
    ren = hdf_get_value(enode, "rendable", NULL);
    mat = hdf_get_value(enode, "material", NULL);
    smat = hdf_get_value(enode, "shadow_material", NULL);
    pos = hdf_get_value(enode, "position", "0 0 0");

    DynamicEntity *e = calloc(1, sizeof(DynamicEntity));
    if (!e) return nerr_raise(NERR_NOMEM, "alloc entity");

    e->base.name = strdup(ename);
    e->base.typeid = ENTITY_TYPE_DYNAMIC;
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

    MatAsset *m = (MatAsset*)e->base.material;
    MatEntry *me;
    for (int i = 0; i < m->entries->num; i++) {
        uListGet(m->entries, i, (void**)&me);
        if (e->base.receive_shadows) mast_mat_add_item_int(me, "receive_shadows", 1);
        else mast_mat_add_item_int(me, "receive_shadows", 0);
    }

    e->rotation = vec4_from_string(hdf_get_value(enode, "rotation", "0 0 0 1"));
    e->scale = vec3_from_string(hdf_get_value(enode, "scale", "1 1 1"));

    e->phy.m = mcs_get_float_value(enode, "weight", 0);
    e->phy.g = vec3_from_string(hdf_get_value(enode, "gravity", "0 -9.8 0"));
    e->phy.v = vec3_from_string(hdf_get_value(enode, "velocity", "0 0 0"));
    e->phy.a = vec3_from_string(hdf_get_value(enode, "acceleration", "0 0 0"));

    e->phy.fa = vec3_from_string(hdf_get_value(enode, "force_a", "0 0 0"));
    e->phy.fb = vec3_from_string(hdf_get_value(enode, "force_b", "0 0 0"));
    e->phy.fc = vec3_from_string(hdf_get_value(enode, "force_c", "0 0 0"));
    e->phy.f = vec3_zero();

    e->act.fa_on = false;
    e->act.fb_on = false;
    e->act.fc_on = false;

    e->me = NULL;

    *pe = (RendEntity*)e;

    return STATUS_OK;

errorexit:
    mentity_dynamic_free(e);
    return nerr_pass(err);
}

NEOERR* mentity_dynamic_update(RendEntity *p, float dt)
{
    DynamicEntity *e = (DynamicEntity*)p;

    vec3 fg = vec3_mul(e->phy.g, e->phy.m);
    e->phy.f = fg;

    if (!vec3_equ(e->phy.f, vec3_zero())) {
        e->phy.a = vec3_div(e->phy.f, e->phy.m);
    } else {
        e->phy.a = vec3_zero();
    }

    vec3 at2 = vec3_mul(e->phy.a, pow(dt, 2.0));
    vec3 v0t = vec3_mul(e->phy.v, dt);
    vec3 s = vec3_add(v0t, vec3_div(at2, 2));

    e->phy.v = vec3_add(e->phy.v, vec3_mul(e->phy.a, dt));
    e->base.position = vec3_add(e->base.position, s);

    e->act.fa_on = false;
    e->act.fb_on = false;
    e->act.fc_on = false;

    return STATUS_OK;
}

EntityDriver dynamic_entity_driver = {
    .typename = "dynamic",
    .new = mentity_dynamic_new,
    .onact = NULL,
    .update = mentity_dynamic_update,
    .free = mentity_dynamic_free,
};
