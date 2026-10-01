#include "../../header/arena/arena_heap.h"
#include "test_runner.h"

#define TEST_DEFAULT_CAPACITY (KiB(1))

#define EXPECTED_VALID_PTR "Expected a valid pointer"
#define EXPECTED_POS "Expected position did not match"
#define EXPECTED_EMPTY_ARENA "Expected arena to be empty"
#define EXPECTED_NON_EMPTY_ARENA "Expected arena to not be empty"

#define CHECK_POS(arena, expected) \
    assert(arena->pos == expected && EXPECTED_POS)

#define CHECK_VALID_PTR(ptr) \
    assert(ptr != NULL && EXPECTED_VALID_PTR)

#define CHECK_VALID_VALUE(ptr, expected) \
    assert(ptr != NULL && *ptr == expected && EXPECTED_VALID_PTR)

#define CHECK_EMPTY_ARENA(arena) \
    assert(arena_is_empty(arena) && EXPECTED_EMPTY_ARENA)

#define CHECK_NON_EMPTY_ARENA(arena) \
    assert(!arena_is_empty(arena) && EXPECTED_NON_EMPTY_ARENA)

#undef TEST_CONFIG
#define TEST_CONFIG(arena) \
    if (SHOULD_CLEAR) arena_clear(arena);

#undef TEST_RESET
#define TEST_RESET(arena) \
    SHOULD_CLEAR = true;

bool SHOULD_CLEAR = true;

void test_push(Arena* arena);
void test_clear(Arena* arena);
void test_capacity(Arena* arena);

int main(void) {
    /* Init */
    Arena* arena = arena_new(TEST_DEFAULT_CAPACITY);

    /* Run tests */
    {
        SHOULD_CLEAR = false;
        RUN_TEST(test_push, arena)
    }
    {
        SHOULD_CLEAR = false;
        RUN_TEST(test_clear, arena)
    }
    {
        SHOULD_CLEAR = true;
        RUN_TEST(test_capacity, arena)
    }

    return 0;
}

void test_push(Arena* arena) {
    /* Arrange */
    const char expectedChar = 'a';
    const short expectedShort = 0;
    const int expectedInt = 1;
    const float expectedFloat = 2.0f;
    const double expectedDouble = 3.0;
    char* expectedStringPtr = "MyArenaString";

    char* charPtr = arena_push(arena, sizeof(char), false);
    short* shortPtr = arena_push(arena, sizeof(short), true);
    int* intPtr = arena_push(arena, sizeof(int), false);
    float* floatPtr = arena_push(arena, sizeof(float), false);
    double* doublePtr = arena_push(arena, sizeof(double), false);
    char* strPtr = arena_push(arena, strlen(expectedStringPtr) + 1, false);

    /* Act */
    *charPtr = expectedChar;
    // *shortPtr = expectedShort; // zero flag is set to true for shorts, and so this SHOULD already be zero
    *intPtr = expectedInt;
    *floatPtr = expectedFloat;
    *doublePtr = expectedDouble;
    strPtr = expectedStringPtr;

    /* Assert */
    CHECK_VALID_VALUE(charPtr, expectedChar);
    CHECK_VALID_VALUE(shortPtr, expectedShort);
    CHECK_VALID_VALUE(intPtr, expectedInt);
    CHECK_VALID_VALUE(floatPtr, expectedFloat);
    CHECK_VALID_VALUE(doublePtr, expectedDouble);
    CHECK_VALID_VALUE(strPtr, *expectedStringPtr); // Checks just the first character
    assert(CHECK_STR(expectedStringPtr, strPtr) && "Expected strings to match");
}

void test_clear(Arena* arena) {
    /* Assume test_push was ran previously */
    CHECK_NON_EMPTY_ARENA(arena);
    arena_clear(arena);
    CHECK_EMPTY_ARENA(arena);
}

void test_capacity(Arena* arena) {
    CHECK_EMPTY_ARENA(arena);
    void* badPtr = arena_push(arena, ARENA_REMAINING_SIZE(arena), true);
    CHECK_VALID_PTR(badPtr);
}