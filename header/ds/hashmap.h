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

/* An open adress hashmap using double hashing and the 32-bit FNV1a algorithim (enhanced double hashing). Capacity should always be a prime number to be compatible with double hashing. */
typedef struct {
    /* Current size of the map */
    hash_size_t size;
    /* Current capacity of the map */
    hash_size_t capacity;
    /* Array of pairs that will resize dynamically when the size becomes greater than the current capacity */
    HashPair* pairs;
} HashMap;

/* Initialize a hashmap with a given capacity. Note that the provided capacity will remain untouched if it is already a prime number, and will otherwise be rounded up to the next closest prime number. */
void hashmap_init(HashMap* map, hash_size_t capacity);
HashPair* hashmap_update(HashMap* map, const char* key, void* value);
HashPair* hashmap_get(HashMap* map, const char* key);
static inline void hashmap_free(HashMap* map) {
    free(map->pairs);
}
#endif /* CGPL_HASHMAP_H_ */