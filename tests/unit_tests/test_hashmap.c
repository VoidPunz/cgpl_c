#include <string.h>

#include "test_runner.h"
#include "../../header/ds/hashmap.h"

#define TEST_HASHMAP_DEFAULT_CAPACITY 2

#undef TEST_CONFIG
#define TEST_CONFIG(map) \
    map.isResizable = IS_RESIZABLE; \
    if (SHOULD_RESIZE) hashmap_resize(&map, TEST_HASHMAP_DEFAULT_CAPACITY); \
    if (SHOULD_CLEAR) hashmap_clear(&map); \

#undef TEST_RESET
#define TEST_RESET(map) \
    SHOULD_RESIZE = true; \
    SHOULD_CLEAR = true; \
    IS_RESIZABLE = true;

bool SHOULD_RESIZE = true;
bool SHOULD_CLEAR = true;
bool IS_RESIZABLE = true;

#define CHECK_PAIR(pair, expectedKey, expectedValue) \
    assert(CHECK_STR(pair->key, expectedKey) && "Keys should match"); \
    assert(pair->value == expectedValue && "Values should match");

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

    pair = hashmap_update(map, key, expected);
    CHECK_PAIR(pair, key, expected)
    assert(map->size == 1 && "Map size should be one");

    expected = (void*)0x2000;
    pair = hashmap_update(map, key, expected);
    CHECK_PAIR(pair, key, expected)
    assert(map->size == 1 && "Map size should still be one");

    // Assert that we cannot insert into a NULL map
    pair = hashmap_update(NULL, key, expected);
    assert(pair == NULL && "Invalid map should return null");
}

void test_get(HashMap* map) {
    const char* key = "test";
    void* expected = (void*)0x1000;
    HashPair* pair = NULL;

    hashmap_update(map, key, expected);
    pair = hashmap_get(map, key);

    assert(CHECK_STR(pair->key, key) && "Keys should match");
    CHECK_PAIR(pair, key, expected)
}

void test_clear(HashMap* map) {
    // Arrange
    const char *_a = "a", *_b = "b", *_c = "c";
    const void* value = (void*)0x1000;
    HashPair* pair = NULL;

    // Act
    hashmap_resize(map, 3);
    hashmap_update(map, _a, value);
    hashmap_update(map, _b, value);
    hashmap_update(map, _c, value);

    hashmap_clear(map);

    // Assert
    pair = hashmap_get(map, _a);
    assert(pair == NULL && "Pair should be null");
    pair = hashmap_get(map, _b);
    assert(pair == NULL && "Pair should be null");
    pair = hashmap_get(map, _c);
    assert(pair == NULL && "Pair should be null");
}

void test_alphabet(HashMap* map) {
    // Arrange
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    const size_t alphabetSize = sizeof(alphabet);
    char keys[alphabetSize - 1][alphabetSize];
    // Act
    hashmap_resize(map, alphabetSize * 2);
    for (int i = 0; i < alphabetSize - 1; i++) {
        for (int j = 0; j <= i; j++)
            keys[i][j] = *(alphabet + j);
        memset(keys[i] + i + 1, '\0', alphabetSize - i);
        hashmap_update(map, keys[i], (void*)i);
    }
    hashmap_print(map);
    // Assert
    for (int i = 0; i < alphabetSize - 1; i++) {
        const HashPair* pair = hashmap_get(map, keys[i]);
        assert(pair != NULL && "Pair should not be null here");
        CHECK_PAIR(pair, keys[i], (void*)i)
    }
}

int main() {
    FORCE_NEVER_SAVE_IN_BUFFER()

    // Init
    HashMap map;
    hashmap_init(&map, TEST_HASHMAP_DEFAULT_CAPACITY, IS_RESIZABLE);

    // Run tests
    {
        SHOULD_RESIZE = false;
        RUN_TEST(test_resize, map)
    }
    RUN_TEST(test_update, map)
    {
        SHOULD_RESIZE = false;
        IS_RESIZABLE = false;
        RUN_TEST(test_clear, map)
    }
    {
        SHOULD_RESIZE = false;
        RUN_TEST(test_alphabet, map)
    }
    return 0;
}