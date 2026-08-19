#include "../header/cgplstate.h"

void cgpl_state_init(CGPLState* state) {
    if (state == NULL) return;
    const bool isDynamic = true, canShrink = false;
    hashmap_init(&state->symbolMap, CGPL_STATE_INIT_HASHMAP_SIZE, isDynamic, canShrink);
}