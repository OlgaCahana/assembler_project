#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

/* Creates and initializes a new empty SymbolTable. Returns NULL if fails to create. */
SymbolTable* symbolTableCreate() {
    SymbolTable *table = (SymbolTable*)malloc(sizeof(struct symbolTable));
    if (table == NULL) {
        return NULL;
    }

    table->head = NULL;
    return table;
}

/* Destroys and frees the memory associated with the SymbolTable, including that of the Symbols stored in it. */
void symbolTableDestroy(SymbolTable *table) {
    SymbolTableNode *curr;
    SymbolTableNode *next;

    if (table == NULL) {
        return;
    }

    for (curr = table->head; curr != NULL; curr = next) {
        next = curr->next;
        symbolDestroy(curr->symbol);
        free(curr);
    }

    free(table);
}

/* Adds the given Symbol to the SymbolTable.
 * Returns SYMBOL_TABLE_FAILURE_ALREADY_EXISTS if a symbol with the same label already exists in the SymbolTable.
 * Returns SYMBOL_TABLE_SUCCESS upon success*/
SymbolTableResult symbolTableAddUnique(SymbolTable *table, Symbol *symbol) {
    SymbolTableNode *node;
    Symbol *dummy;

    if (table == NULL || symbol == NULL) {
        return SYMBOL_TABLE_FAILURE_NULL_ARGUMENT;
    }

    if (symbolTableGet(table, symbol->label, &dummy) == SYMBOL_TABLE_SUCCESS) {
        return SYMBOL_TABLE_FAILURE_ALREADY_EXISTS;
    }

    node = (SymbolTableNode*)malloc(sizeof(struct symbolTableNode));
    if (node == NULL) {
        return SYMBOL_TABLE_FAILURE_MEMORY_ALLOCATION;
    }

    node->symbol = symbol;
    node->next = table->head;
    table->head = node;

    return SYMBOL_TABLE_SUCCESS;
}

/* Adds the given Symbol to the SymbolTable. Allows adding a symbols with the same label
 * Returns SYMBOL_TABLE_SUCCESS upon success*/
SymbolTableResult symbolTableAdd(SymbolTable *table, Symbol *symbol) {
    SymbolTableNode *node;

    if (table == NULL || symbol == NULL) {
        return SYMBOL_TABLE_FAILURE_NULL_ARGUMENT;
    }

    node = (SymbolTableNode*)malloc(sizeof(struct symbolTableNode));
    if (node == NULL) {
        return SYMBOL_TABLE_FAILURE_MEMORY_ALLOCATION;
    }

    node->symbol = symbol;
    node->next = table->head;
    table->head = node;

    return SYMBOL_TABLE_SUCCESS;
}

/* Retrieves a Symbol with the given label from the SymbolTable.
 * Returns SYMBOL_TABLE_FAILURE_NOT_EXISTS if no such symbol exist in the SymbolTable.
 * Returns SYMBOL_TABLE_SUCCESS upon success */
SymbolTableResult symbolTableGet(SymbolTable *table, const char *label, Symbol **symbol) {
    SymbolTableNode *node;

    if (table == NULL || label == NULL || symbol == NULL) {
        return SYMBOL_TABLE_FAILURE_NULL_ARGUMENT;
    }

    for (node = table->head; node != NULL; node = node->next) {
        if (strcmp(label, node->symbol->label) == 0) {
            *symbol = node->symbol;
            return SYMBOL_TABLE_SUCCESS;
        }
    }

    return SYMBOL_TABLE_FAILURE_NOT_EXISTS;
}