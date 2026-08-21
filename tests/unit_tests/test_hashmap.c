#include <string.h>

#include "test_runner.h"
#include "../../header/ds/hashmap.h"

#define TEST_HASHMAP_DEFAULT_CAPACITY 2

#undef TEST_CONFIG
#define TEST_CONFIG(map) \
    map->dynamic = IS_DYNAMIC; \
    map->shrinking = IS_SHRINKING; \
    if (SHOULD_CLEAR) hashmap_clear(map); \
    if (SHOULD_RESIZE) hashmap_resize(map, TEST_HASHMAP_DEFAULT_CAPACITY); \

#undef TEST_RESET
#define TEST_RESET(map) \
    SHOULD_RESIZE = true; \
    SHOULD_CLEAR = true; \
    IS_DYNAMIC = true; \
    IS_SHRINKING = true;

bool SHOULD_RESIZE = true;
bool SHOULD_CLEAR = true;
bool IS_DYNAMIC = true;
bool IS_SHRINKING = true;

#define EXPECT_SUCCESSFUL_RESIZE "Expected successful resize"
#define EXPECT_SUCCESSFUL_INSERT "Expected successful insert"
#define EXPECT_SUCCESSFUL_GET "Expected successful get"
#define EXPECT_FAILED_INSERT "Expected failed insert"
#define EXPECT_FAILED_GET "Expected failed get"
#define EXPECT_CAPACITY_TO_MATCH_VALUE "Expected capacity to match value"
#define EXPECT_SIZE_TO_MATCH_VALUE "Expected size to match value"

#define CHECK_PAIR(pair, expectedKey, expectedValue) \
    assert(CHECK_STR(pair->key, expectedKey) && "Keys should match"); \
    assert(pair->value == expectedValue && "Values should match");

void test_resize(HashMap* map);
void test_update(HashMap* map);
void test_clear(HashMap* map);
void test_alphabet(HashMap* map);
void test_resize_shrink(HashMap* map);

int main() {
    FORCE_NEVER_SAVE_IN_BUFFER()

    // Init
    HashMap map;
    hashmap_init(&map, TEST_HASHMAP_DEFAULT_CAPACITY, IS_DYNAMIC, IS_SHRINKING);
    assert(map.size == 0 && "Size should be zero");
    assert(map.capacity == TEST_HASHMAP_DEFAULT_CAPACITY && EXPECT_CAPACITY_TO_MATCH_VALUE);

    // Run tests
    {
        SHOULD_RESIZE = false;
        RUN_TEST(test_resize, &map)
    }
    RUN_TEST(test_update, &map)
    {
        SHOULD_RESIZE = false;
        IS_DYNAMIC = false;
        RUN_TEST(test_clear, &map)
    }
    {
        SHOULD_RESIZE = false;
        RUN_TEST(test_alphabet, &map)
    }
    {
        IS_DYNAMIC = false;
        RUN_TEST(test_resize_shrink, &map)
    }
    return 0;
}

void test_resize(HashMap* map) {
    const hash_size_t value = 16, expected = 17;
    hashmap_resize(map, value);
    assert(map->capacity == expected && "New capacity should match expected capacity");
    hashmap_resize(map, expected);
    assert(map->capacity == expected && "Expected capacity should not change if new capacity is identical");
}

void test_update(HashMap* map) {
    const char* key = "test";
    void* expected = (void*)0x1000;
    HashPair* pair = NULL;

    assert((pair = hashmap_update(map, key, expected)) != NULL && EXPECT_SUCCESSFUL_INSERT);
    CHECK_PAIR(pair, key, expected)
    assert(map->size == 1 && "Map size should be one");

    expected = (void*)0x2000;
    assert((pair = hashmap_update(map, key, expected)) != NULL && EXPECT_SUCCESSFUL_INSERT);
    CHECK_PAIR(pair, key, expected)
    assert(map->size == 1 && "Map size should still be one");

    // Assert that we cannot insert into a NULL map
    assert((pair = hashmap_update(NULL, key, expected)) == NULL && EXPECT_FAILED_INSERT);
}

void test_get(HashMap* map) {
    const char* key = "test";
    void* expected = (void*)0x1000;
    HashPair* pair = NULL;

    assert(hashmap_update(map, key, expected) != NULL && EXPECT_SUCCESSFUL_INSERT);
    assert((pair = hashmap_get(map, key)) != NULL && EXPECT_SUCCESSFUL_GET);

    CHECK_PAIR(pair, key, expected)
}

void test_clear(HashMap* map) {
    // Arrange
    const char *_a = "a", *_b = "b", *_c = "c";
    const void* value = (void*)0x1000;

    // Act
    assert(hashmap_resize(map, 3) && EXPECT_SUCCESSFUL_RESIZE);
    assert(hashmap_update(map, _a, value) != NULL && EXPECT_SUCCESSFUL_INSERT);
    assert(hashmap_update(map, _b, value) != NULL && EXPECT_SUCCESSFUL_INSERT);
    assert(hashmap_update(map, _c, value) != NULL && EXPECT_SUCCESSFUL_INSERT);

    hashmap_clear(map);

    // Assert
    assert(hashmap_get(map, _a) == NULL && EXPECT_FAILED_GET);
    assert(hashmap_get(map, _b) == NULL && EXPECT_FAILED_GET);
    assert(hashmap_get(map, _c) == NULL && EXPECT_FAILED_GET);
}

void test_alphabet(HashMap* map) {
    // Arrange
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    const size_t alphabetSize = sizeof(alphabet);
    char keys[alphabetSize - 1][alphabetSize];
    // Act
    assert(hashmap_resize(map, alphabetSize * 2) && EXPECT_SUCCESSFUL_RESIZE);
    for (int i = 0; i < alphabetSize - 1; i++) {
        for (int j = 0; j <= i; j++)
            keys[i][j] = alphabet[j];
        memset(keys[i] + i + 1, '\0', alphabetSize - i);
        assert(hashmap_update(map, keys[i], (void*)i) != NULL && EXPECT_SUCCESSFUL_INSERT);
    }
    // Assert
    for (int i = 0; i < alphabetSize - 1; i++) {
        const HashPair* pair = NULL;
        assert((pair = hashmap_get(map, keys[i])) != NULL && EXPECT_SUCCESSFUL_GET);
        CHECK_PAIR(pair, keys[i], (void*)i)
    }
}

void test_resize_shrink(HashMap* map) {
    const hash_size_t initValue = 7, shrinkValue = 5, smallestPrimeValue = 2;
    const char keys[] = "ABCDEFG";
    assert(sizeof(keys) - 1 == initValue && "Number of elements in the keys array must match initValue");

    assert(map->size == 0 && "Initial size should be zero");
    assert(hashmap_resize(map, initValue) && EXPECT_SUCCESSFUL_RESIZE);
    assert(map->capacity == initValue && EXPECT_CAPACITY_TO_MATCH_VALUE);

    for (hash_size_t i = 0; i < map->capacity; i++)
        assert(hashmap_update(map, keys + i, (void*)i) != NULL && EXPECT_SUCCESSFUL_INSERT);
    
    assert(map->size == map->capacity && "Used size should match the capacity");

    assert(hashmap_resize(map, shrinkValue) && EXPECT_SUCCESSFUL_RESIZE);
    assert(map->capacity == shrinkValue && EXPECT_CAPACITY_TO_MATCH_VALUE);
    assert(map->size == map->capacity && "Size should match capacity");

    assert(hashmap_resize(map, initValue) && EXPECT_SUCCESSFUL_RESIZE);
    assert(map->capacity == initValue && EXPECT_CAPACITY_TO_MATCH_VALUE);
    assert(map->size == shrinkValue && "Size should remain unchanged after increased capacity");

    map->shrinking = false;
    assert(!hashmap_resize(map, smallestPrimeValue) && "Should not be able to shrink when shrinking is disabled");
    assert(map->capacity == initValue && EXPECT_CAPACITY_TO_MATCH_VALUE);
    assert(map->size == shrinkValue && "Size should be unchanged");
}