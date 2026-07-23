#ifndef CGPL_HASHTABLE_H_
#define CGPL_HASHTABLE_H_

#define HASHTABLE_SIZE 1024

typedef struct {
    void* value;
    int key;
} KeyValuePair;

typedef struct {
    /* Dynamic allocations of pairs */
    KeyValuePair* pairs;
    /* Size of pair array */
    size_t size;
} Hashtable;

#endif /* CGPL_HASHTABLE_H_ */