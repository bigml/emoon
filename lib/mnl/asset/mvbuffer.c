#include "mheads.h"

static VbufferAsset* mvb_node_new()
{
    VbufferAsset *v = calloc(1, sizeof(VbufferAsset));

    return v;
}

static VbufferSurface* mvb_surface_new(mesh *m)
{
    VbufferSurface *s = calloc(1, sizeof(VbufferSurface));

    glGenBuffers(1, &s->vertex_vbo);
    glGenBuffers(1, &s->triangle_vbo);

    s->num_verticies = m->num_verts;
    s->num_triangles = m->num_triangles;

    /* Position Normal Tangent Binormal Uvs Color      */
    /* 3        3      3       3        2   4     = 18 */
    float *vb_data = malloc(sizeof(float) * m->num_verts * 18);

    for(int i = 0; i < m->num_verts; i++) {
        vec3 pos = m->verticies[i].position;
        vec3 norm = m->verticies[i].normal;
        vec3 tang = m->verticies[i].tangent;
        vec3 bino = m->verticies[i].binormal;
        vec2 uvs = m->verticies[i].uvs;
        vec4 col = m->verticies[i].color;

        vb_data[(i*18)+0] = pos.x;
        vb_data[(i*18)+1] = pos.y;
        vb_data[(i*18)+2] = pos.z;

        vb_data[(i*18)+3] = norm.x;
        vb_data[(i*18)+4] = norm.y;
        vb_data[(i*18)+5] = norm.z;

        vb_data[(i*18)+6] = tang.x;
        vb_data[(i*18)+7] = tang.y;
        vb_data[(i*18)+8] = tang.z;

        vb_data[(i*18)+9] = bino.x;
        vb_data[(i*18)+10] = bino.y;
        vb_data[(i*18)+11] = bino.z;

        vb_data[(i*18)+12] = uvs.x;
        vb_data[(i*18)+13] = uvs.y;

        vb_data[(i*18)+14] = col.x;
        vb_data[(i*18)+15] = col.y;
        vb_data[(i*18)+16] = col.z;
        vb_data[(i*18)+17] = col.w;
    }

    glBindBuffer(GL_ARRAY_BUFFER, s->vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * s->num_verticies * 18,
                 vb_data, GL_STATIC_DRAW);
    free(vb_data);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->triangle_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * s->num_triangles * 3,
                 m->triangles, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return s;
}

static void mvb_surface_delete(VbufferSurface *s)
{
    glDeleteBuffers(1 , &s->vertex_vbo);
    glDeleteBuffers(1 , &s->triangle_vbo);

    free(s);
}

static  void mvb_add_mesh(VbufferAsset *v, mesh *m)
{
    VbufferSurface *s = mvb_surface_new(m);

    v->num_surfaces++;
    v->surfaces = realloc(v->surfaces, sizeof(VbufferSurface*) * v->num_surfaces);
    v->surfaces[v->num_surfaces-1] = s;

    //mtc_dbg("%s added a mesh to GPU memory", v->base.name);
}

static void mvb_add_group(model *omode, mesh *amesh, int vert_index,
                   int_list *tlist, vertex_list *vlist)
{
    amesh->num_verts = vert_index;
    amesh->num_triangles = tlist->num_items / 3;

    amesh->verticies = malloc(sizeof(vertex) * amesh->num_verts);
    for (int i = 0; i < amesh->num_verts; i++) {
        amesh->verticies[i] = vertex_list_get(vlist, i);
    }

    amesh->triangles = malloc(sizeof(int) * amesh->num_triangles * 3);
    for (int i = 0; i < amesh->num_triangles * 3; i++) {
        amesh->triangles[i] = int_list_get(tlist, i);
    }

    omode->num_meshes++;
    omode->meshes = realloc(omode->meshes, sizeof(mesh*) * omode->num_meshes);
    omode->meshes[omode->num_meshes-1] = amesh;
}

static void mvb_add_model(VbufferAsset *v, model *m)
{
    for (int i = 0; i < m->num_meshes; i++) {
        mvb_add_mesh(v, m->meshes[i]);
    }

    //mtc_dbg("%s added a model[%d meshes] to GPU memory", v->base.name, m->num_meshes);
}

NEOERR* mast_vb_obj_load(char *dir, char *name, RendAsset **a)
{
    char *buf, *line, fname[PATH_MAX];
    NEOERR *err;

    if (dir) snprintf(fname, sizeof(fname), "%s%s", dir, name);
    else strncpy(fname, name, sizeof(fname));

    err = ne_load_file(fname, &buf);
    if (err != STATUS_OK) return nerr_pass(err);

    ULIST *lines;
    err = mstr_array_fast_split(&lines, buf, "\n", 1048576);
    if (err != STATUS_OK) return nerr_pass(err);

    mesh *amesh = NULL;
    model *omode = calloc(1, sizeof(model));

    int vert_index = 0;
    bool has_normal_data = false, has_texcoord_data = false;

    vertex_list *vlist = vertex_list_new();
    int_list    *tlist = int_list_new();

    vertex_list *vdata = vertex_list_new();
    vertex_hashtable *vhash = vertex_hashtable_new(4096);

    VbufferAsset *v = mvb_node_new();

    MCS_NOT_NULLB(omode, v);

    MLIST_ITERATE(lines, line) {
        line = neos_strip(line);

        if (line[0] == 'g') {
            if (amesh) {
                mvb_add_group(omode, amesh, vert_index, tlist, vlist);
            }

            vert_index = 0;
            vertex_hashtable_delete(vhash);
            vertex_list_delete(vlist);
            int_list_delete(tlist);

            vlist = vertex_list_new();
            tlist = int_list_new();
            vhash = vertex_hashtable_new(4096);

            amesh = calloc(1, sizeof(mesh));
        }

        mfmt_obj_parse_line(line, &vert_index,
                            vlist, tlist,
                            vdata, vhash,
                            &has_normal_data, &has_texcoord_data);
    }

    uListDestroy(&lines, 0);
    SAFE_FREE(buf);

    mvb_add_group(omode, amesh, vert_index, tlist, vlist);

    vertex_list_delete(vlist);
    int_list_delete(tlist);

    vertex_list_delete(vdata);
    vertex_hashtable_delete(vhash);

    if (omode->num_meshes <= 0)
        return nerr_raise(NERR_ASSERT, "file %s have no groups", fname);

    if (!has_normal_data) model_generate_normals(omode);
    if (!has_texcoord_data) model_generate_texcoords_cylinder(omode);
    model_generate_tangents(omode);

    mvb_add_model(v, omode);

    *a = (RendAsset*)v;

    model_delete(omode);

    return STATUS_OK;
}

void mast_vb_unload(void *p)
{
    if (!p) return;

    VbufferAsset *v = p;

    for (int i = 0; i < v->num_surfaces; i++) {
        mvb_surface_delete(v->surfaces[i]);
    }

    //mtc_dbg("%s freed %d meshes GPU memory", v->base.name, v->num_surfaces);

    /*
     * RendAsset node freed by caller: masset_node_unload().
     */
    //free(v);
}
