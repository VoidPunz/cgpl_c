#ifndef CGPL_HASHMAP_H_
#define CGPL_HASHMAP_H_

typedef struct {
    size_t key;
    void* value;
} HashPair;

typedef struct {
    HashPair* pairs;
    size_t size;
} HashMap;

#endif /* CGPL_HASHMAP_H_ */