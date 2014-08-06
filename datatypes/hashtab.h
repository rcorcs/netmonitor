#ifndef __HASHTAB
#define __HASHTAB

#ifdef __cplusplus
extern "C" {
#endif

#include "primitives.h"

typedef struct __hash_node {
   byte_t *key;
   uint_t klen;
   byte_t *val;
   uint_t vlen;
   struct __hash_node *next;
} hash_node_t;

hash_node_t *create_hash_node(byte_t *key, uint_t klen, byte_t *val, uint_t vlen, hash_node_t *next);
hash_node_t *alloc_hash_node(uint_t klen, uint_t vlen);
void destroy_hash_node(hash_node_t **hnode);

typedef struct __hash_tab {
   struct __hash_node **buckets;
   uint_t len;
   uint_t entries;
   bool_t (*compare)(byte_t *, uint_t, byte_t *, uint_t);
} hash_tab_t;

ulong_t hash(const byte_t *data, uint_t dlen);

hash_tab_t *create_hash_tab(uint_t len, bool_t (*compare_func)(byte_t *, uint_t, byte_t *, uint_t));
void destroy_hash_tab(hash_tab_t **htab);
void destroy_hash_tab_nodes(hash_tab_t *htab);

bool_t hash_tab_compare(byte_t *key, uint_t klen, byte_t *tkey, uint_t tklen);
hash_node_t *hash_tab_lookup(hash_tab_t *htab, byte_t *key, uint_t klen);
void hash_tab_insert(hash_tab_t *htab, hash_node_t *new_hnode);
void hash_tab_remove(hash_tab_t *htab, byte_t *key, uint_t klen);

#ifdef __cplusplus
}
#endif

#endif
