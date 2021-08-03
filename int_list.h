#ifndef INT_LIST_H
#define INT_LIST_H

#include <stdbool.h>

typedef struct intNode IntNode;
typedef struct intList IntList;

/* A node in the linked list of int's */
struct intNode {
    int n;
    IntNode *next;
};

/* A linked list of int's */
struct intList  {
    IntNode *head;
    IntNode *tail;
};

/* Creates and initializes a new empty IntList. */
IntList* intListCreate();

/* Destroys and frees the memory associated with the IntList. */
bool intListDestroy(IntList *list);

/* Adds the given integer to the tail of the list. */
bool intListEnqueue(IntList *list, int n);

/* Removes an integer from the head of the list, and stores it in the given integer. */
bool intListDequeue(IntList *list, int *n);

#endif /* INT_LIST_H */
