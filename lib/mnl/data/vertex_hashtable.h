/**
*** :: Vertex List ::
***
***   Hashtable for verticies
***   used to check duplicates
***   in various asset loaders.
***
**/

#ifndef vertex_hashtable_h
#define vertex_hashtable_h

#include "vertex_list.h"
#include "int_list.h"
#include "mheads.h"

__BEGIN_DECLS

typedef struct {
  vertex_list* keys;
  int_list* values;
} vertex_bucket;

typedef struct {
  vertex_bucket* items;
  int table_size;
} vertex_hashtable;

int vertex_hash(vertex_hashtable* ht, vertex v);

vertex_hashtable* vertex_hashtable_new(int size);
void vertex_hashtable_delete(vertex_hashtable* ht);

void vertex_hashtable_set(vertex_hashtable* ht, vertex key, int value);
int vertex_hashtable_get(vertex_hashtable* ht, vertex key);

__END_DECLS
#endif
