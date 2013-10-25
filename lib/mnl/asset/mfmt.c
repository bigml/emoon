#include "mheads.h"

void mfmt_obj_parse_line(char *line, int *vindex,
                         vertex_list *vlist, int_list *tlist,
                         vertex_list *vdata, vertex_hashtable *vhash,
                         bool *has_normal_data, bool *has_texcoord_data)
{
    char comment[512];
    char matlib[512];
    char object[512];
    //char group[512];
    char material[512];
    float px, py, pz, tx, ty, nx, ny, nz;
    int smoothing_group;
    int pi1, ti1, ni1, pi2, ti2, ni2, pi3, ti3, ni3;

    static int num_pos = 0, num_norm = 0, num_tex = 0;
    int vert_index = *vindex;
    
    if (sscanf(line, "# %512s", comment) == 1) {
      /* Comment, do nothing */
    }
    
    else if (sscanf(line, "mtllib %512s", matlib) == 1) {
      /* Do Nothing */
    }
    
    else if (sscanf(line, "o %512s", object) == 1) {
      /* Do Nothing */
    }
    
    else if (sscanf(line, "v %f %f %f", &px, &py, &pz) == 3) {
    
      while(vdata->num_items <= num_pos) { vertex_list_push_back(vdata, vertex_new()); }
      vertex vert = vertex_list_get(vdata, num_pos);
      vert.position = vec3_new(px, py, pz);
      vertex_list_set(vdata, num_pos, vert);
      num_pos++;
    }
    
    else if (sscanf(line, "vt %f %f", &tx, &ty) == 2) {
    
      while(vdata->num_items <= num_tex) { vertex_list_push_back(vdata, vertex_new()); }
      vertex vert = vertex_list_get(vdata, num_tex);
      vert.uvs = vec2_new(tx, ty);
      vertex_list_set(vdata, num_tex, vert);
      num_tex++;
    }
    
    else if (sscanf(line, "vn %f %f %f", &nx, &ny, &nz) == 3) {
    
      while(vdata->num_items <= num_norm) { vertex_list_push_back(vdata, vertex_new()); }
      vertex vert = vertex_list_get(vdata, num_norm);
      vert.normal = vec3_new(nx, ny, nz);
      vertex_list_set(vdata, num_norm, vert);
      num_norm++;
    }
    
    else if (sscanf(line, "usemtl %512s", material) == 1) {
      /* Do Nothing */
    }
    
    else if (sscanf(line, "s %i", &smoothing_group) == 1) {
      /* Smoothing group, do nothing */
    }
    
    else if (sscanf(line, "f %i/%i/%i %i/%i/%i %i/%i/%i", &pi1, &ti1, &ni1, &pi2, &ti2, &ni2, &pi3, &ti3, &ni3) == 9) {
      *has_normal_data = true;
      *has_texcoord_data = true;
      
      /* OBJ file indicies start from one, have to subtract one */
      pi1--; ti1--; ni1--; pi2--; ti2--; ni2--; pi3--; ti3--; ni3--;
      
      vertex v1, v2, v3;
      v1.position = vertex_list_get(vdata, pi1).position;
      v1.uvs = vertex_list_get(vdata, ti1).uvs;
      v1.normal = vertex_list_get(vdata, ni1).normal;
      
      v2.position = vertex_list_get(vdata, pi2).position;
      v2.uvs = vertex_list_get(vdata, ti2).uvs;
      v2.normal = vertex_list_get(vdata, ni2).normal;
      
      v3.position = vertex_list_get(vdata, pi3).position;
      v3.uvs = vertex_list_get(vdata, ti3).uvs;
      v3.normal = vertex_list_get(vdata, ni3).normal;
      
      int v1_id = vertex_hashtable_get(vhash, v1);
      if ( v1_id == -1 ) {
        vertex_hashtable_set(vhash, v1, vert_index);
        vertex_list_push_back(vlist, v1);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v1_id);
      }
      
      int v2_id = vertex_hashtable_get(vhash, v2);
      if ( v2_id == -1 ) {
        vertex_hashtable_set(vhash, v2, vert_index);
        vertex_list_push_back(vlist, v2);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v2_id);
      }
      
      int v3_id = vertex_hashtable_get(vhash, v3);
      if ( v3_id == -1 ) {
        vertex_hashtable_set(vhash, v3, vert_index);
        vertex_list_push_back(vlist, v3);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v3_id);
      }
      
    }
    
    else if (sscanf(line, "f %i//%i %i//%i %i//%i", &pi1, &ni1, &pi2, &ni2, &pi3, &ni3) == 6) {
      *has_normal_data = true;
      *has_texcoord_data = false;
      
      /* OBJ file indicies start from one, have to subtract one */
      pi1--; ni1--; pi2--; ni2--; pi3--; ni3--;
      
      vertex v1, v2, v3;
      v1.position = vertex_list_get(vdata, pi1).position;
      v1.uvs = vec2_zero();
      v1.normal = vertex_list_get(vdata, ni1).normal;
      
      v2.position = vertex_list_get(vdata, pi2).position;
      v2.uvs = vec2_zero();
      v2.normal = vertex_list_get(vdata, ni2).normal;
      
      v3.position = vertex_list_get(vdata, pi3).position;
      v3.uvs = vec2_zero();
      v3.normal = vertex_list_get(vdata, ni3).normal;
      
      int v1_id = vertex_hashtable_get(vhash, v1);
      if ( v1_id == -1 ) {
        vertex_hashtable_set(vhash, v1, vert_index);
        vertex_list_push_back(vlist, v1);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v1_id);
      }
      
      int v2_id = vertex_hashtable_get(vhash, v2);
      if ( v2_id == -1 ) {
        vertex_hashtable_set(vhash, v2, vert_index);
        vertex_list_push_back(vlist, v2);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v2_id);
      }
      
      int v3_id = vertex_hashtable_get(vhash, v3);
      if ( v3_id == -1 ) {
        vertex_hashtable_set(vhash, v3, vert_index);
        vertex_list_push_back(vlist, v3);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v3_id);
      }
      
    }
    
    else if (sscanf(line, "f %i/%i %i/%i %i/%i", &pi1, &ti1, &pi2, &ti2, &pi3, &ti3) == 6) {
      *has_normal_data = false;
      *has_texcoord_data = true;
      
      /* OBJ file indicies start from one, have to subtract one */
      pi1--; ti1--; pi2--; ti2--; pi3--; ti3--;
      
      vertex v1, v2, v3;
      v1.position = vertex_list_get(vdata, pi1).position;
      v1.uvs = vertex_list_get(vdata, ti1).uvs;
      v1.normal = vec3_zero();
      
      v2.position = vertex_list_get(vdata, pi2).position;
      v2.uvs = vertex_list_get(vdata, ti2).uvs;
      v2.normal = vec3_zero();
      
      v3.position = vertex_list_get(vdata, pi3).position;
      v3.uvs = vertex_list_get(vdata, ti3).uvs;
      v3.normal = vec3_zero();
      
      int v1_id = vertex_hashtable_get(vhash, v1);
      if ( v1_id == -1 ) {
        vertex_hashtable_set(vhash, v1, vert_index);
        vertex_list_push_back(vlist, v1);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v1_id);
      }
      
      int v2_id = vertex_hashtable_get(vhash, v2);
      if ( v2_id == -1 ) {
        vertex_hashtable_set(vhash, v2, vert_index);
        vertex_list_push_back(vlist, v2);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v2_id);
      }
      
      int v3_id = vertex_hashtable_get(vhash, v3);
      if ( v3_id == -1 ) {
        vertex_hashtable_set(vhash, v3, vert_index);
        vertex_list_push_back(vlist, v3);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v3_id);
      }
      
    }
    
    else if (sscanf(line, "f %i %i %i", &pi1, &pi2, &pi3) == 3) {
      *has_normal_data = false;
      *has_texcoord_data = false;
      
      /* OBJ file indicies start from one, have to subtract one */
      pi1--; pi2--; pi3--;
      
      vertex v1, v2, v3;
      v1.position = vertex_list_get(vdata, pi1).position;
      v1.uvs = vec2_zero();
      v1.normal = vec3_zero();
      
      v2.position = vertex_list_get(vdata, pi2).position;
      v2.uvs = vec2_zero();
      v2.normal = vec3_zero();
      
      v3.position = vertex_list_get(vdata, pi3).position;
      v3.uvs = vec2_zero();
      v3.normal = vec3_zero();
      
      int v1_id = vertex_hashtable_get(vhash, v1);
      if ( v1_id == -1 ) {
        vertex_hashtable_set(vhash, v1, vert_index);
        vertex_list_push_back(vlist, v1);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v1_id);
      }
      
      int v2_id = vertex_hashtable_get(vhash, v2);
      if ( v2_id == -1 ) {
        vertex_hashtable_set(vhash, v2, vert_index);
        vertex_list_push_back(vlist, v2);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v2_id);
      }
      
      int v3_id = vertex_hashtable_get(vhash, v3);
      if ( v3_id == -1 ) {
        vertex_hashtable_set(vhash, v3, vert_index);
        vertex_list_push_back(vlist, v3);
        int_list_push_back(tlist, vert_index);
        vert_index++;
      } else {
        int_list_push_back(tlist, v3_id);
      }
      
    }

    *vindex = vert_index;
}
