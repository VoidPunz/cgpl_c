#ifndef CGPL_STACK_H_
#define CGPL_STACK_H_

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils.h"
#include "../debug.h"
#include "../types.h"

/* Compile time initialization for a Stack object. Buffer will be uninitialized, properly initialize by calling stack_init */
#define STACK(_size) ((Stack){.top = -1, .size = _size, .buffer = NULL})

typedef uint32_t stack_size_t;
typedef int64_t stack_top_t;

/*  A dynamically allocated generic stack */
typedef struct {
    /* Index of the current top index */
    stack_top_t top;
    /* The size of the stack buffer */
    stack_size_t size;
    /* Static buffer of data of a given size */
    void** buffer;
} Stack;

/* Initialize a given stack */
void stack_init(Stack* stack, stack_size_t size);
/* Push data onto the top of the stack. Returns the index if successful, or -1 if the stack is full. */
stack_top_t stack_push(Stack* stack, void* data);
/* Pop data off the top of the stack. Returns the data pointer at the top or NULL if the stack is empty. */
void* stack_pop(Stack* stack);
/* Peek the current data at the top of the stack. Returns a const pointer to the data or NULL if empty. */
const void* stack_peek(Stack* stack);
/* Clear the stack of all data. If data is the sole references to dynamic memory, calling this function will cause memory leak(s). */
static inline void stack_clear(Stack* stack) {
    memset(stack->buffer, 0, sizeof(void*) * (size_t)stack->size);
    stack->top = -1;
}
/* Checks if the stack is full */
static inline bool stack_is_full(Stack* stack) {
    return stack->buffer == NULL || stack->top == (stack_top_t)stack->size - 1;
}
/* Checks if the stack is empty */
static inline bool stack_is_empty(Stack* stack) {
    return stack->buffer == NULL || stack->top == -1;
}
/* Returns the number of elements in the stack. Returns 0 if the stack is NULL */
static inline stack_size_t stack_count(Stack* stack) {
    return stack == NULL ? 0 : stack->top - 1;
}

#endif /* CGPL_STACK_H_ */