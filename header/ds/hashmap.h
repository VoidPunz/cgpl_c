#ifndef CGPL_HASHMAP_H_
#define CGPL_HASHMAP_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils.h"
#include "../error.h"
#include "../debug.h"

typedef uint32_t hash_size_t;

typedef struct HashPair {
    /* Associated key */
    const char* key;
    /* Associated value */
    void* value;
} HashPair;

typedef struct {
    /* Current size of the map */
    hash_size_t size;
    /* Current capacity of the map */
    hash_size_t capacity;
    /* Array of pairs that will resize dynamically when the size becomes greater than the current capacity */
    HashPair* pairs;
} HashMap;

extern hash_size_t g_PrimeLookup[];

/* Initialize a hashmap with a given capacity. IMPORTANT: The capacity should always be a prime number. */
void hashmap_init(HashMap* map, hash_size_t capacity);
HashPair* hashmap_insert(HashMap* map, const char* key, void* value);
HashPair* hashmap_get(HashMap* map, const char* key);
static inline void hashmap_free(HashMap* map) {
    free(map->pairs);
}
#endif /* CGPL_HASHMAP_H_ */