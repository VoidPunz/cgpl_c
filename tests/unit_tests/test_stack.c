#include "test_runner.h"
#include "../../header/ds/stack.h"

#undef TEST_CONFIG
#define TEST_CONFIG(stack, _size) \
    if (stack->buffer != NULL) free(stack->buffer); \
    stack_init(stack, _size); \

#define EXPECTED_SUCCESSFUL_PUSH "Expected successful push"
#define EXPECTED_SUCCESSFUL_POP "Expected successful pop"
#define EXPECTED_SUCCESSFUL_PEEK "Expected successful peek"
#define EXPECTED_FAILED_PUSH "Expected failed push"
#define EXPECTED_FAILED_POP "Expected failed pop"
#define EXPECTED_EMPTY "Expected stack to be empty"
#define EXPECTED_NOT_EMPTY "Expected stack to NOT be empty"
#define EXPECTED_FULL "Expected stack to be full"
#define EXPECTED_NOT_FULL "Expected stack NOT to be full"
#define EXPECTED_MATCHING_VALUES "Expected values to match"

#define TRY_PUSH(stack, data) \
    assert((stack_push(stack, data) != -1) && EXPECTED_SUCCESSFUL_PUSH)

#define TRY_PUSH_GRAB(stack, data, top) \
    assert(((top = stack_push(stack, data)) != -1) && EXPECTED_SUCCESSFUL_PUSH)

#define TRY_FAIL_PUSH(stack, data) \
    assert((stack_push(stack, data) == -1) && EXPECTED_FAILED_PUSH)

#define TRY_PEEK(stack) \
    assert((stack_peek() != NULL) && EXPECTED_SUCCESSFUL_PEEK);

#define TRY_POP(stack) \
    assert((stack_pop(stack) != NULL) && EXPECTED_SUCCESSFUL_POP)

#define TRY_POP_GRAB(stack, value) \
    assert(((value = stack_pop(stack)) != NULL) && EXPECTED_SUCCESSFUL_POP)

#define TRY_FAIL_POP(stack) \
    assert((stack_pop(stack) == NULL) && EXPECTED_FAILED_POP);

#define CHECK_IS_EMPTY(stack) \
    assert(stack_is_empty(stack) && EXPECTED_EMPTY)

#define CHECK_IS_NOT_EMPTY(stack) \
    assert(!stack_is_empty(stack) && EXPECTED_NOT_EMPTY);

#define CHECK_IS_FULL(stack) \
    assert(stack_is_full(stack) && EXPECTED_FULL)

#define CHECK_IS_NOT_FULL(stack) \
    assert(!stack_is_full(stack) && EXPECTED_NOT_FULL)

typedef uint32_t test_value_t;

#define TEST_STACK_DEFALT_SIZE 10
#define TEST_VALUE_DEFAULT ((test_value_t)100)

void test_push(Stack* stack);
void test_peek(Stack* stack);
void test_clear(Stack* stack);
void test_pop(Stack* stack);
void test_empty(Stack* stack);

int main() {
    FORCE_NEVER_SAVE_IN_BUFFER()

    Stack stack = STACK(0);

    RUN_TEST(test_push, &stack, TEST_STACK_DEFALT_SIZE)
    RUN_TEST(test_empty, &stack, TEST_STACK_DEFALT_SIZE)
    RUN_TEST(test_clear, &stack, 3)
    RUN_TEST(test_pop, &stack, TEST_STACK_DEFALT_SIZE)

    return 0;
}

void test_push(Stack* stack) {
    TRY_FAIL_PUSH(stack, NULL);
    for (stack_size_t i = 0; i < stack->size; i++)
        TRY_PUSH(stack, TO_VOIDPTR(TEST_VALUE_DEFAULT));

    CHECK_IS_FULL(stack);
    TRY_FAIL_PUSH(stack, TO_VOIDPTR(TEST_VALUE_DEFAULT));
}

void test_peek(Stack* stack) {
    const void* expected = TO_VOIDPTR(TEST_VALUE_DEFAULT);
    TRY_PUSH(stack, expected);
    CHECK_IS_NOT_EMPTY(stack);
}

void test_clear(Stack* stack) {
    const test_value_t expected = TEST_VALUE_DEFAULT;
    uint32_t top = 0;

    assert(stack->size == 3 && "Size should be three");
    assert(stack != NULL && stack->buffer != NULL);

    TRY_PUSH_GRAB(stack, TO_VOIDPTR(expected), top);
    assert(top == 0 && "Top should be zero");
    
    TRY_PUSH_GRAB(stack, TO_VOIDPTR(expected), top);
    assert(top == 1 && "Top should be one");
    
    TRY_PUSH_GRAB(stack, TO_VOIDPTR(expected), top);
    assert(top == 2 && "Top should be two");

    CHECK_IS_FULL(stack);
    stack_clear(stack);
    CHECK_IS_EMPTY(stack);
}

void test_pop(Stack* stack) {
    const test_value_t expected = TEST_VALUE_DEFAULT;
    void* temp = NULL;

    CHECK_IS_EMPTY(stack);
    TRY_FAIL_POP(stack);
    TRY_PUSH(stack, TO_VOIDPTR(expected));
    TRY_POP_GRAB(stack, temp);
    assert(FROM_VOIDPTR(temp, test_value_t) == expected && EXPECTED_MATCHING_VALUES);

    for (stack_size_t i = 0; i < stack->size; i++)
        TRY_PUSH(stack, TO_VOIDPTR(i + 1));

    while (!stack_is_empty(stack))
        TRY_POP(stack);
}

void test_empty(Stack* stack) {
    const void* expected = TO_VOIDPTR(TEST_VALUE_DEFAULT);

    CHECK_IS_EMPTY(stack);
    TRY_PUSH(stack, TO_VOIDPTR(expected));
    CHECK_IS_NOT_EMPTY(stack);
    TRY_POP(stack);
    CHECK_IS_EMPTY(stack);
}