#ifndef CGPL_LIST_H_
#define CGPL_LIST_H_

#include <stdlib.h>
#include "error.h"
#include "debug.h"

typedef struct List_Node {
    void* data;
    struct List_Node* next;
} List_Node;

/* Create a new node with pointer data. */
List_Node* list_new(void* data);
/* Returns number of nodes for a given node. */
size_t list_count(List_Node* node);
/* Connects the second node to the first node. Overrides 'next' field. Use this if you have the tail of a list instead of using list_append. O(1) time complexity. */
static inline void list_connect(List_Node* firstNode, List_Node* secondNode) {
    if (firstNode == NULL) return;
    /* Second node can be NULL */
    firstNode->next = secondNode;
}
/* Appends a given node to the back of the list of a given root node. O(N) time complexity. */
void list_append(List_Node* node, List_Node* appendNode);
/* Frees all nodes from a given root node including the root node. Frees the dynamic data if any. */
void list_free_cascade(List_Node* node);
/* Frees the single node without cascading. Frees the dynamic data if any. */
void list_free(List_Node* node);
/* Cascadingly free all the nodes in the list without deallocating the associated data. */
void list_free_standalone_cascade(List_Node* node);
/* Frees just the node without deallocating the associated data. */
void list_free_standalone(List_Node* node);
#ifdef DEBUG
/* Print a list of nodes using a given printer function. The supplied node can never be NULL. */
static void list_print(List_Node* node, const char* (*printer_ptr)(const List_Node*)) {
        while (node != NULL) {
        printf("%s\n", printer_ptr(node));
        node = node->next;
    }
}
#endif

#endif /* CGPL_LIST_H_ */