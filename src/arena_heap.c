#include "../header/arena/arena_heap.h"

Arena* arena_new(arena_capacity_t capacity) {
    Arena* arena = (Arena*)malloc(capacity);
    arena->capacity = capacity;
    arena->pos = ARENA_BASE_OFFSET;
    return arena;
}

void* arena_push(Arena* arena, arena_capacity_t size, bool zero) {
    arena_capacity_t aligned = ALIGN_UP_POW2(arena->pos, ARENA_ALLIGN);
    arena_capacity_t pos = aligned + size;
    if (pos > arena->capacity) return NULL;

    void* data = arena + aligned;
    arena->pos += aligned;

    if (zero) {
        memset(data, 0, size);
    }

    return data;
}