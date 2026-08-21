#include "../header/ds/stack.h"

void stack_init(Stack* stack, stack_size_t size) {
    if (stack == NULL) return;
    stack->top = -1;
    stack->size = size;
    stack->buffer = size == 0 ? NULL : (void**)malloc(sizeof(void*) * size);
}

stack_top_t stack_push(Stack* stack, void* data) {
    // We don't allow null values to avoid confusion with the NULL fail-state return type of stack_pop
    if (data == NULL) return -1;
    if (stack == NULL || stack_is_full(stack)) return -1;
    stack->buffer[++stack->top] = data;
    return stack->top;
}

void* stack_pop(Stack* stack) {
    if (stack == NULL || stack_is_empty(stack)) return NULL;
    return stack->buffer[stack->top--];
}

const void* stack_peek(Stack* stack) {
    if (stack == NULL || stack_is_empty(stack)) return NULL;
    return stack->buffer[stack->top];
}