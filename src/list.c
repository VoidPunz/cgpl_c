#include "../header/ds/list.h"

ListNode* list_new(void* data) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    if (node == NULL) ERROR_BAD_ALLOC;
    node->data = data;
    node->next = NULL;
    return node;
}

size_t list_count(ListNode* node) {
    size_t c = 0;
    while (node != NULL) {
        c++;
        node = node->next;
    }
    return c;
}

void list_append(ListNode* node, ListNode* appendNode) {
    if (node == NULL) return;
    while (node->next != NULL) {
        node = node->next;
    }
    node->next = appendNode;
}

void list_free_cascade(ListNode** node) {
    ListNode* tempNode = NULL;
    while (*node != NULL) {
        tempNode = (*node)->next;
        list_free(node);
        node = &tempNode;
    }
}

void list_free(ListNode** node) {
    if (*node == NULL) return;
    if ((*node)->data != NULL) free((*node)->data);
    (*node)->next = NULL;
    free(*node);
    *node = NULL;
}

void list_free_standalone_cascade(ListNode** node) {
    ListNode* tempNode = NULL;
    while (*node != NULL) {
        tempNode = (*node)->next;
        list_free_standalone(node);
        node = &tempNode;
    }
}

void list_free_standalone(ListNode** node) {
    if (*node != NULL) free(*node);
    *node = NULL;
}

#ifdef DEBUG
    void list_print(ListNode* node, const char* (*printer_ptr)(const ListNode*)) {
            while (node != NULL) {
            printf("%s\n", printer_ptr(node));
            node = node->next;
        }
    }

    void list_default_printer(const ListNode* node) {
        char buffer[100] = {'\0'};
        sprintf(buffer, "node: %p, next: %p, data: %p\n", node, node->next, node->data);
    }
#endif