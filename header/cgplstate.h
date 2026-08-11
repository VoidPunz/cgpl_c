#ifndef CGPL_CGPLSTATE_H_
#define CGPL_CGPLSTATE_H_

#include "types.h"
#include "ds/hashmap.h"

#define CGPL_STATE_INIT_HASHMAP_SIZE 16

typedef struct {
    HashMap symbolMap;
} CGPLState;

/* Initialise a state */
void cgpl_state_init(CGPLState* state);
/* Free a state */
static inline void cgpl_state_free(CGPLState** state) {
    if (state == NULL) return;
    hashmap_free(&((*state)->symbolMap));
    free(*state);
    *state = NULL;
}

#endif /* CGPL_CGPLSTATE_H_ */