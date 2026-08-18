#include "../header/cgplstate.h"

void cgpl_state_init(CGPLState* state) {
    if (state == NULL) return;
    hashmap_init(&state->symbolMap, CGPL_STATE_INIT_HASHMAP_SIZE, true);
}