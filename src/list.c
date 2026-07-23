#include "../header/list.h"

List_Node* list_new(void* data) {
    List_Node* node = (List_Node*)malloc(sizeof(List_Node));
    if (node == NULL) ERROR_BAD_ALLOC;
    node->data = data;
    node->next = NULL;
    return node;
}

size_t list_count(List_Node* node) {
    size_t c = 0;
    while (node != NULL) {
        c++;
        node = node->next;
    }
    return c;
}

void list_append(List_Node* node, List_Node* appendNode) {
    if (node == NULL) return;
    while (node->next != NULL) {
        node = node->next;
    }
    node->next = appendNode;
}

void list_free_cascade(List_Node* node) {
    List_Node* tempNode = NULL;
    while (node != NULL) {
        tempNode = node->next;
        list_free(node);
        node = tempNode;
    }
}

void list_free(List_Node* node) {
    if (node == NULL) return;
    if (node->data != NULL) free(node->data);
    node->next = NULL;
    free(node);
}

void list_free_standalone_cascade(List_Node* node) {
    List_Node* tempNode = NULL;
    while (node != NULL) {
        tempNode = node->next;
        list_free_standalone(node);
        node = tempNode;
    }
}

void list_free_standalone(List_Node* node) {
    if (node == NULL) return;
    node->next = NULL;
    free(node);
}