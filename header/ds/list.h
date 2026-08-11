#ifndef CGPL_LIST_H_
#define CGPL_LIST_H_

#include <stdlib.h>
#include <string.h>
#include "../error.h"
#include "../debug.h"

#define CGPL_PRINT_BUFFER_SIZE 100

typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;

/* Create a new node with pointer data. */
ListNode* list_new(void* data);
/* Returns number of nodes for a given node. */
size_t list_count(ListNode* node);
/* Connects the second node to the first node. Overrides 'next' field. Use this if you have the tail of a list instead of using list_append. O(1) time complexity. */
static inline void list_connect(ListNode* firstNode, ListNode* secondNode) {
    if (firstNode == NULL) return;
    /* Second node can be NULL */
    firstNode->next = secondNode;
}
/* Appends a given node to the back of the list of a given root node. O(N) time complexity. */
void list_append(ListNode* node, ListNode* appendNode);
/* Frees all nodes from a given root node including the root node. Frees the dynamic data if any. */
void list_free_cascade(ListNode* node);
/* Frees the single node without cascading. Frees the dynamic data if any. */
void list_free(ListNode* node);
/* Cascadingly free all the nodes in the list without deallocating the associated data. */
void list_free_standalone_cascade(ListNode* node);
/* Frees just the node without deallocating the associated data and without cascading. */
void list_free_standalone(ListNode* node);
#ifdef DEBUG
/* Print a list of nodes using a given printer function. The supplied node can never be NULL. */
static void list_print(ListNode* node, const char* (*printer_ptr)(const ListNode*)) {
        while (node != NULL) {
        printf("%s\n", printer_ptr(node));
        node = node->next;
    }
}
#endif

#endif /* CGPL_LIST_H_ */