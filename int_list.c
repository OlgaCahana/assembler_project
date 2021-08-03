#include <stdlib.h>
#include "int_list.h"

/* Creates and initializes a new empty IntList. */
IntList* intListCreate() {
    IntList *list = (IntList*)malloc(sizeof(struct intList));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;

    return list;
}

/* Destroys and frees the memory associated with the IntList. */
bool intListDestroy(IntList *list) {
    IntNode *curr;
    IntNode *next;

    if (list == NULL) {
        return false;
    }

    for (curr = list->head; curr != NULL; curr = next) {
        next = curr->next;
        free(curr);
    }

    free(list);
    return true;
}

/* Adds the given integer to the tail of the list. */
bool intListEnqueue(IntList *list, int n) {
    IntNode *node;

    if (list == NULL) {
        return false;
    }

    node = (IntNode*)malloc(sizeof(struct intNode));
    if (node == NULL) {
        return false;
    }

    node->n = n;
    node->next = NULL;

    if (list->tail != NULL) {
        list->tail->next = node;
    }

    list->tail = node;

    if (list->head == NULL) {
        list->head = node;
    }

    return true;
}

/* Removes an integer from the head of the list, and stores it in the given integer. */
bool intListDequeue(IntList *list, int *n) {
    IntNode *head;

    if (list == NULL) {
        return false;
    }

    if (n == NULL) {
        return false;
    }

    head = list->head;
    if (head == NULL) {
        return false;
    }

    *n = head->n;

    list->head = head->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }

    free(head);
    return true;
}