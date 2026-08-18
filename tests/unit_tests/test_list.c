#include "test_runner.h"
#include "../../header/ds/list.h"

#undef TEST_CONFIG
#define TEST_CONFIG(headNode) \
    if (SHOULD_CLEAR) list_free_standalone_cascade(&headNode->next);

#undef TEST_RESET
#define TEST_RESET(headNode) \
    SHOULD_CLEAR = true;

bool SHOULD_CLEAR = true;

void test_connect(ListNode* headNode) {
    ListNode* node = list_new(NULL);
    list_connect(headNode, node);
    assert(headNode->next == node && "Next node should match");
}

void test_append(ListNode* headNode) {
    ListNode* n1 = list_new(NULL);
    ListNode* n2 = list_new(NULL);
    ListNode* n3 = list_new(NULL);

    list_append(headNode, n1);
    list_append(headNode, n2);
    list_append(headNode, n3);

    assert(headNode->next == n1 && "headNode->next == n1");
    assert(n1->next == n2 && "n1->next == n2");
    assert(n2->next == n3 && "n2->next == n3");
    assert(n3->next == NULL && "n3 is tail");
}

void test_count(ListNode* headNode) {
    assert(list_count(headNode) == 1 && "Size should be one");
    ListNode* n1 = list_new(NULL);
    ListNode* n2 = list_new(NULL);
    ListNode* n3 = list_new(NULL);

    list_append(headNode, n1);
    list_append(headNode, n2);
    list_append(headNode, n3);

    assert(list_count(headNode) == 4 && "Size should be four");
}

int main() {
    FORCE_NEVER_SAVE_IN_BUFFER()

    // Init
    ListNode* headNode = list_new(NULL);

    // Run tests
    RUN_TEST(test_count, headNode)
    RUN_TEST(test_append, headNode)
    RUN_TEST(test_connect, headNode)
    
    return 0;
}