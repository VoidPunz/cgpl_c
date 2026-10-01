#ifndef CGPL_ARENA_STANDARD_H_
#define CGPL_ARENA_STANDARD_H_

#include "../error.h"
#include "arena_utils.h"

typedef u64 arena_capacity_t;

#define ARENA_BASE_OFFSET ((arena_capacity_t)sizeof(Arena))
#define ARENA_ALLIGN (sizeof(void*))
/* Maximum possible size of an allocation assuming the arena is empty. Helper macro, generally better to use ARENA_REMAINING_SIZE instead. */
#define ARENA_MAX_SIZE(arena) (arena->capacity - ARENA_BASE_OFFSET)
/* The size of the remaining unallocated arena */
#define ARENA_REMAINING_SIZE(arena) (arena->capacity - arena->pos)

/* Magicalbat inspired implementation of a heap memory arena: https://gist.github.com/Magicalbat/4e085cadeed46c7b6f917ea9e9220d6a */
typedef struct {
    /* The allocated memory capacity for this arena */
    arena_capacity_t capacity;
    /* The current position within the arena */
    arena_capacity_t pos;
} Arena;

/* Dynamically allocates a new arena on the heap */
Arena* arena_new(arena_capacity_t capacity);
/* Tries to push (reserve) data of a given size onto the arena. If 'zero' is true, the data is initialized to 0 before being returned. */
void* arena_push(Arena* arena, arena_capacity_t size, bool zero);
/* Pops data off the arena */
static inline void arena_pop(Arena* arena, arena_capacity_t size) {
    arena->pos -= MIN(size, arena->pos - ARENA_BASE_OFFSET);
}
/* Pop data off until a given position in the arena */
static inline void arena_pop_to(Arena* arena, arena_capacity_t pos) {
    arena_pop(arena, pos < arena->pos ? arena->pos - pos : 0);
}
/* Resets all data in the arena to 0. This should effectively invalidate all pre-existing pointers to the arena. */
static inline void arena_clear(Arena* arena) {
    arena_pop_to(arena, ARENA_BASE_OFFSET);
}
/* Returns true if the arena is empty */
static inline bool arena_is_empty(Arena* arena) {
    return arena->pos <= ARENA_BASE_OFFSET;
}

#endif /* CGPL_ARENA_STANDARD_H_ */