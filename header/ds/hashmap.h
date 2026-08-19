#ifndef CGPL_HASHMAP_H_
#define CGPL_HASHMAP_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
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

/* A dynamic open adress hashmap using double hashing and the 32-bit FNV1a algorithim (enhanced double hashing). Capacity should always be a prime number to be compatible with double hashing. */
typedef struct {
    /* Current size of the map */
    hash_size_t size;
    /* Current capacity of the map. Will always be a prime number. */
    hash_size_t capacity;
    /* Array of pairs that will resize dynamically when the size becomes greater than the current capacity */
    HashPair** pairs;
    /* A flag to guarantee the hashmap will not resize during updates if set to true. You can still manually resize with hashmap_resize. */
    bool dynamic;
    /* A flag that guarantees that the hashmap cannot shrink and thus not lose data if set to true. */
    bool shrinking;
} HashMap;

/* Initialize a hashmap with a given capacity. Note that the provided capacity will remain untouched if it is already a prime number, and will otherwise be rounded up to the next closest prime number. The actual capacity will always be >= to the given capacity. */
void hashmap_init(HashMap* map, hash_size_t capacity, bool dynamic, bool shrinking);
/* Resizes the hashmap for a given capacity. O(1) if map->size == 0 or new and old capacity are identical, but otherwise a new dynamic array is allocated, and all existing indices will have to be recalculated. If you know the exact perfect size for the intended purpose, multiplying the capacity by 2 or setting the isResizable falg to false will ensure no resize is ever called. NOTE: If the capacity shrinks, this could invalidate existing pointers as they may be freed. Returns true if successful resize. */
bool hashmap_resize(HashMap* map, hash_size_t capacity);
/* Updates (or inserts) a key in the hashmap. Returns the updated pair, or NULL on failure. */
const HashPair* hashmap_update(HashMap* map, const char* key, void* value);
/* Clear all key-value pairs on the hashmap */
void hashmap_clear(HashMap* map);
/* Get a key-value pair associated with a given key. Returns NULL if none found. */
const HashPair* hashmap_get(HashMap* map, const char* key);
/* Frees a dynamic map by deallocating all its pairs. */
static inline void hashmap_free(HashMap* map) {
    for (hash_size_t i = 0; i < map->capacity; i++)
        if (map->pairs[i] != NULL) free(map->pairs[i]);
}
void hashmap_print(const HashMap* map);

#endif /* CGPL_HASHMAP_H_ */