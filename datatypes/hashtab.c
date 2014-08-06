#include "hashtab.h"

#include <string.h>
#include <stdlib.h>


ulong_t hash(const byte_t *data, uint_t dlen)
{
    //I've chosen multiples of the PI and E constants just because I like maths
	ulong_t hashval = 31415926+27182818; // (PI+E)*(10^8)
	uint_t i;

	for(i = 0; i<dlen; i++){
		hashval = ((hashval << 3) + hashval ) + data[i]; //magical hashing function
	}
	return hashval;
}

hash_tab_t *create_hash_tab(uint_t len, bool_t (*compare_func)(byte_t *, uint_t, byte_t *, uint_t))
{
    uint_t i;
    hash_tab_t *htab = (hash_tab_t *)malloc( sizeof(hash_tab_t) );
    htab->buckets = (hash_node_t **)malloc(len*sizeof(hash_node_t *));
    for(i=0;i<len;i++) htab->buckets[i] = NULL;
    htab->len = len;
    htab->entries = 0;
    htab->compare = compare_func;
    return htab;
}

void destroy_hash_tab(hash_tab_t **htab)
{
    free((*htab)->buckets);
    free(*htab);
    *htab = 0;
}

void destroy_hash_tab_nodes(hash_tab_t *htab)
{
    uint_t i;
    hash_node_t *hnode;
    hash_node_t *temphnode;
    for(i=0; i<htab->len; i++){
        hnode = htab->buckets[i];
        while(hnode!=0){
            temphnode = hnode->next;
            destroy_hash_node(&hnode);
            hnode = temphnode;
        }
        htab->buckets[i] = 0;
    }
}

hash_node_t *create_hash_node(byte_t *key, uint_t klen, byte_t *val, uint_t vlen, hash_node_t *next)
{
   hash_node_t *hnode = (hash_node_t *)malloc(sizeof(hash_node_t));
   hnode->klen=klen;
   hnode->vlen=vlen;
   //hnode->key = (byte_t*)malloc(klen);
   //hnode->val = (byte_t*)malloc(vlen);
   //memcpy(hnode->key, key, klen);
   //memcpy(hnode->val, val, vlen);
   hnode->key = key;
   hnode->val = val;

   hnode->next = next;
   return hnode;
}

hash_node_t *alloc_hash_node(uint_t klen, uint_t vlen)
{
    hash_node_t *hnode = (hash_node_t *)malloc(sizeof(hash_node_t));
    hnode->klen=klen;
    hnode->vlen=vlen;
    hnode->key = (byte_t*)malloc(klen);
    hnode->val = (byte_t*)malloc(vlen);
    hnode->next = NULL;
    return hnode;
}

void destroy_hash_node(hash_node_t **hnode)
{
   if(hnode && (*hnode)){
      free( (*hnode)->key );
      free( (*hnode)->val );
      free( (*hnode) );
      *hnode = 0;
   }
}

bool_t hash_tab_compare(byte_t *key, uint_t klen, byte_t *tkey, uint_t tklen)
{
   bool_t res = BOOL_FALSE;
   uint_t i;
   if(klen==tklen){
      res = BOOL_TRUE;
      for(i = 0; i<klen; i++){
          if( key[i]!=tkey[i] ){
              res = BOOL_FALSE;
              break;
          }
      }
   }
   return res;
}

hash_node_t *hash_tab_lookup(hash_tab_t *htab, byte_t *key, uint_t klen)
{
   ulong_t keyhash = hash(key, klen)%htab->len;
   hash_node_t *hnode;
   bool_t (*compare_func)(byte_t *, uint_t, byte_t *, uint_t);
   if(htab->compare)
      compare_func = htab->compare;
   else compare_func = hash_tab_compare;
   for(hnode = htab->buckets[keyhash]; hnode!=0; hnode = hnode->next){
       if( compare_func(hnode->key, hnode->klen, key, klen) ){
           return hnode;
       }
   }
   return 0;
}

void hash_tab_insert(hash_tab_t *htab, hash_node_t *new_hnode)
{
   ulong_t keyhash = hash(new_hnode->key, new_hnode->klen)%htab->len;
   hash_node_t *hnode;
   bool_t (*compare_func)(byte_t *, uint_t, byte_t *, uint_t);
   if(htab->compare)
      compare_func = htab->compare;
   else compare_func = hash_tab_compare;
   hnode = htab->buckets[keyhash];
   if(hnode!=0){
      while(hnode->next!=0){
         if( compare_func(hnode->key, hnode->klen, new_hnode->key, new_hnode->klen) )
            return;
         else hnode = hnode->next;
      }
      if( compare_func(hnode->key, hnode->klen, new_hnode->key, new_hnode->klen) )
            return;

      hnode->next = new_hnode;
   }else htab->buckets[keyhash] = new_hnode;
   htab->entries++;
}

void hash_tab_remove(hash_tab_t *htab, byte_t *key, uint_t klen)
{
   ulong_t keyhash = hash(key, klen)%htab->len;
   hash_node_t *hnode;
   bool_t (*compare_func)(byte_t *, uint_t, byte_t *, uint_t);
   if(htab->compare)
      compare_func = htab->compare;
   else compare_func = hash_tab_compare;
   hnode = htab->buckets[keyhash];
   if(hnode!=0){
      if( compare_func(hnode->key, hnode->klen, key, klen) ){
         destroy_hash_node(&htab->buckets[keyhash]);
         htab->buckets[keyhash] = hnode->next;
         htab->entries--;
      }else {
         while(hnode->next!=0){
            if( compare_func(hnode->next->key, hnode->next->klen, key, klen) ){
               destroy_hash_node(&hnode->next);
               hnode->next = hnode->next->next;
               htab->entries--;
            }
         }
      }
   }
}
