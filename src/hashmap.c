#include "../header/ds/hashmap.h"

typedef uint64_t fnv_size_t;

/* 64-bit fnv1a offset basis*/
#define FNV_OFFSET_BASIS ((fnv_size_t)0xcbf29ce484222325u)
/* 64-bit fnv1a prime */
#define FNV_PRIME ((fnv_size_t)0x00000100000001b3)

/* Checks if the given number is a prime number */
static bool is_prime(hash_size_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true; // 1 and 2 are the smallest prime numbers
    if (n % 2 == 0 || n % 3 == 0) return false; // Even numbers or numbers divisible by 3 cannot be primes
    for (hash_size_t i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}

/* Finds the next prime number for a given number (prime or not)*/
static hash_size_t next_prime(hash_size_t n) {
    if (n < 2) return 2;
    while (!is_prime(++n));
    return n;
}

/* Makes sure the given number is constrained to be a prime number */
static inline hash_size_t prime(hash_size_t n) {
    return is_prime(n) ? n : next_prime(n);
}

/* Fowler-Noll-Vo-1a 64 bit hash */
static fnv_size_t hash_fnv1a(const HashMap* map, const char* key) {
    fnv_size_t hash = FNV_OFFSET_BASIS;
    while (*key) {
        hash *= FNV_PRIME;
        hash ^= (unsigned char)*key++;
    }
    return hash % map->capacity;
}

/* Performs an enhanced double hash */
static hash_size_t enhanced_double_hash(const HashMap* map, const char* key) {
    const int sd = sizeof(fnv_size_t) - sizeof(hash_size_t);
    const fnv_size_t h0 = hash_fnv1a(map, key);
    hash_size_t h1 = (hash_size_t)h0, h2 = h0 << sd;
    HashPair* pair = NULL;
    do {
        
    } while (pair != NULL);
}

void hashmap_init(HashMap* map, hash_size_t capacity) {
    if (map == NULL) return;
    capacity = prime(capacity);
    map->pairs = (HashPair*)malloc(sizeof(HashPair) * capacity);
    for (hash_size_t i = 0; i < capacity; i++)
        map->pairs[i].value = NULL;
    map->capacity = capacity;
    map->size = 0;
}

static void resize(HashMap* map) {
    hash_size_t capacity = prime(map->capacity * 2);
    HashPair* tempPairs = (HashPair*)malloc(sizeof(HashPair) * capacity);
    memcpy(tempPairs, map->pairs, map->capacity);
    free(map->pairs);
    map->pairs = tempPairs;
    map->capacity = capacity;
}

HashPair* hashmap_update(HashMap* map, const char* key, void* value) {
    if (map == NULL) {
        cgpl_warning("Attempted to insert into a NULL map.");
        return NULL;
    }
    if (map->size + 1 > map->capacity) resize(map);
    // Double hash
    hash_size_t index = enhanced_double_hash(map, key);
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