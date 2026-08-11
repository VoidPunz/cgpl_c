#include "../header/ds/hashmap.h"

/* 32-bit fnv1a offset basis*/
#define FNV_OFFSET_BASIS ((hash_size_t)2166136261u)
/* 32-bit fnv1a prime */
#define FNV_PRIME ((hash_size_t)16777619u)

/* Calculate the next prime number. Panics if the next prime overflows the type hash_size_t. */
static hash_size_t hashmap_calc_next_prime(hash_size_t prime) {
    
}

/* Fowler-Noll-Vo-1a 32 bit hash */
static hash_size_t hash_fnv1a(const HashMap* map, const char* key) {
    hash_size_t hash = FNV_OFFSET_BASIS;
    while (*key) {
        hash *= FNV_PRIME;
        hash ^= (unsigned char)*key++;
    }
    return hash % map->capacity;
}

void hashmap_init(HashMap* map, hash_size_t capacity) {
    if (map == NULL) return;
    map->pairs = (HashPair*)malloc(sizeof(HashPair) * capacity);
    for (hash_size_t i = 0; i < capacity; i++)
        map->pairs[i].value = NULL;
    map->capacity = capacity;
    map->size = 0;
}

static void resize(HashMap* map) {
    hash_size_t capacity = map->capacity * 2;
    HashPair* tempPairs = (HashPair*)malloc(sizeof(HashPair) * capacity);
    memcpy(tempPairs, map->pairs, map->capacity);
    free(map->pairs);
    map->pairs = tempPairs;
    map->capacity = capacity;
}

HashPair* hashmap_insert(HashMap* map, const char* key, void* value) {
    if (map == NULL) {
        cgpl_warning("Attempted to insert into a NULL map.");
        return NULL;
    }
    if (map->size + 1 > map->capacity) resize(map);
    hash_size_t index = hash_fnv1a(map, key);
    HashPair* pair = map->pairs + index;
    #ifdef DEBUG
        if (pair->value == NULL) {
            DEBUG_PRINT("HASHMAP (%s) INSERT: key = %s, value = %p\n", map->capacity, key, value);
        } else {
            DEBUG_PRINT("HASHMAP (%s) UPDATE: [N] key = %s, value = %p | [O] key = %s, value = %p\n", map->capacity, key, value, pair->key, pair->value);
        }
    #endif
    pair->value = value;
    pair->key = key;
    map->size++;
    return pair;
}

HashPair* hashmap_get(HashMap* map, const char* key) {
    if (map == NULL) {
        cgpl_warning("Attempted to insert into a NULL map.");
        return NULL;
    }
    hash_size_t index = hash_fnv1a(map, key);
    return map->pairs + index;
}