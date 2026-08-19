#include "../header/ds/hashmap.h"

typedef uint64_t fnv_size_t;

/* 64-bit fnv1a offset basis*/
#define FNV_OFFSET_BASIS ((fnv_size_t)0xcbf29ce484222325ULL)
/* 64-bit fnv1a prime */
#define FNV_PRIME ((fnv_size_t)0x00000100000001b3ULL)

/* Checks if the given number is a prime number */
static bool is_prime(hash_size_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true; // 2 and 3 are the smallest prime numbers
    if (n % 2 == 0 || n % 3 == 0) return false; // Even numbers or numbers divisible by 3 cannot be primes
    for (hash_size_t i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}

// TODO: Dynamically cache primes in a finite array for faster lookup, or alternatively use a binary search algorithim on precomputed a array of primes
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
static fnv_size_t hash_fnv1a(const char* key) {
    fnv_size_t hash = FNV_OFFSET_BASIS;
    while (*key) {
        hash ^= (unsigned char)*key++;
        hash *= FNV_PRIME;
    }
    return hash;
}

#define MAX_HASH_ITERATION_COEFFECIENT 100

/* Performs an enhanced double hash on a key. Returns an index to the corresponding key. */
static hash_size_t enhanced_double_hash(HashPair** pairs, hash_size_t capacity, const char* key) {
    const fnv_size_t h0 = hash_fnv1a(key);
    const hash_size_t h1 = (hash_size_t)(h0 & 0xFFFFFFFF); // First 32 bits
    const hash_size_t h2 = (hash_size_t)(h0 >> 32); // Last 32 bits
    const hash_size_t step = h2 % (capacity - 1) + 1; // Step and capacity must be coprime. Step cannot be 0 and neither a multiple of the capacity
    hash_size_t index;
    size_t c = 1;
    HashPair* pair;
    while (true) {
        // Shouldnt really needs this
        //if (c > capacity * MAX_HASH_ITERATION_COEFFECIENT) cgpl_error_fatal("Hash for key \"%s\" has exceeded maximum attempts | c > (capacity * %d == %d) | capacity = %d\n", key, MAX_HASH_ITERATION_COEFFECIENT, capacity * MAX_HASH_ITERATION_COEFFECIENT, capacity);
        index = (h1 + c * step) % capacity;
        pair = pairs[index];

        if (pair == NULL) break;
        if (pair->key == NULL) break;
        if (CHECK_STR(pair->key, key)) break;
        c++;
    }
    DEBUG_PRINT("HASH FOR \"%s\" - c: %d h0: %llx, h1: %x h2: %x, i: %u, attempts: %llu\n", key, capacity, h0, h1, h2, index, c);
    return index;
}

static inline HashPair* hashpair_new(const char* key, void* value) {
    HashPair* pair = (HashPair*)malloc(sizeof(HashPair));
    pair->key = key;
    pair->value = value;
    return pair;
}

void hashmap_init(HashMap* map, hash_size_t capacity, bool dynamic) {
    if (map == NULL) return;
    capacity = prime(capacity);
    map->pairs = (HashPair**)calloc(capacity, sizeof(HashPair*));
    if (map->pairs == NULL) ERROR_BAD_ALLOC;
    for (hash_size_t i = 0; i < capacity; i++) {
        map->pairs[i] = hashpair_new(NULL, NULL);
    }
    map->capacity = capacity;
    map->dynamic = dynamic;
    map->size = 0;
}

void hashmap_resize(HashMap* map, hash_size_t capacity) {
    if (map == NULL || map->pairs == NULL) return;
    capacity = prime(capacity);
    if (capacity == map->capacity) return;
    HashPair** tempPairs = (HashPair**)calloc(capacity, sizeof(HashPair*));
    if (tempPairs == NULL) ERROR_BAD_ALLOC;

    // Rehash all indices of the HashPairs from the old array and copy into the new one
    // Copy as many as we can if we don't have enough space (iff capacity < map->capacity)
    if (map->size > 0) {
        for (hash_size_t i = 0; i < MIN(capacity, map->capacity); i++) {
            HashPair* pair = map->pairs[i];
            if (pair == NULL) continue;
            hash_size_t index = enhanced_double_hash(tempPairs, capacity, pair->key);
            tempPairs[index] = pair;
        }

        // Free any allocated HashPairs in the old array that would otherwise be discarded.
        if (capacity < map->capacity)
            for (hash_size_t i = capacity; i < map->capacity; i++) {
                if (map->pairs[i] == NULL) continue;
                free(map->pairs[i]);
            }
    }

    hash_size_t size = MIN(map->size, capacity);
    DEBUG_PRINT("HASHMAP[%p] RESIZE (s=%d, c=%d->%d) %s\n", map, size, map->capacity, capacity, capacity > map->capacity ? "INCREASED" : "DECREASED");
    
    free(map->pairs);
    map->pairs = tempPairs;
    map->capacity = capacity;
    if (map->size > size) cgpl_warning("HASHMAP[%p]: Data has been lost after resize\n", map);
    map->size = size;
}

const HashPair* hashmap_update(HashMap* map, const char* key, void* value) {
    if (map == NULL) {
        cgpl_warning("Attempted to insert into a NULL map.");
        return NULL;
    }
    if (map->dynamic && map->size + 1 > map->capacity / 2) hashmap_resize(map, map->capacity * 2);

    hash_size_t index = enhanced_double_hash(map->pairs, map->capacity, key);

    // Insert
    if (map->pairs[index] == NULL) {
        if (!map->dynamic && map->size + 1 > map->capacity) {
            cgpl_warning("HASHPAIR[%p]: Resize inhibited, new key \"%s\" rejected.\n", map, key);
            return NULL;
        }

        DEBUG_PRINT("HASHMAP[%p] (s=%d,c=%d) INIT: key = \"%s\", value = %p\n", map, map->size, map->capacity, key, value);
        map->pairs[index] = hashpair_new(key, value);
        map->size++;
        return map->pairs[index];
    }

    // Update
    HashPair* pair = map->pairs[index];
    #ifdef DEBUG
        if (pair->key == NULL) {
            DEBUG_PRINT("HASHMAP[%p] (s=%d,c=%d) INSERT: key = \"%s\", value = %p\n", map, map->size, map->capacity, key, value);
        } else {
            DEBUG_PRINT("HASHMAP[%p] (s=%d,c=%d) UPDATE: [N] key = \"%s\", value = %p | [O] key = %s, value = %p\n", map, map->size, map->capacity, key, value, pair->key, pair->value);
        }
    #endif
    pair->key = key;
    pair->value = value;
    return pair;
}

void hashmap_clear(HashMap* map) {
    if (map == NULL) cgpl_error_fatal("Null map");
    if (map->pairs == NULL) cgpl_error_fatal("Null pairs");
    for (hash_size_t i = 0; i < map->capacity; i++) {
        HashPair** pairPtr = map->pairs + i;
        if (*pairPtr != NULL) free(*pairPtr);
        *pairPtr = NULL;
    }
    DEBUG_PRINT("HASHMAP[%p] cleared (%d->0)!\n", map, map->size);
    map->size = 0;
}

const HashPair* hashmap_get(HashMap* map, const char* key) {
    if (key == NULL) return NULL;
    if (map == NULL) {
        cgpl_warning("Attempted to insert into a NULL map.");
        return NULL;
    }
    hash_size_t index = enhanced_double_hash(map->pairs, map->capacity, key);
    return map->pairs[index];
}

void hashmap_print(const HashMap* map) {
    if (map == NULL) return;
    if (map->pairs == NULL) return;
    for (hash_size_t i = 0; i < map->capacity; i++) {
        const HashPair* pair = map->pairs[i];
        if (pair != NULL) 
            printf("%d: %p - %s\n", i, pair->value, pair->key);
        else
            printf("%d: %p\n", i, pair);
    }
}