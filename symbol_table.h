#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "symbol.h"

typedef struct symbolTable SymbolTable;
typedef struct symbolTableNode SymbolTableNode;

/* A node in the symbol table */
struct symbolTableNode {
    Symbol *symbol;
    SymbolTableNode *next;
};

/* A table holding symbols */
struct symbolTable {
    SymbolTableNode *head;
};

/* Return types from the symbol table functions */
typedef enum {
    SYMBOL_TABLE_SUCCESS,
    SYMBOL_TABLE_FAILURE_NULL_ARGUMENT,
    SYMBOL_TABLE_FAILURE_MEMORY_ALLOCATION,
    SYMBOL_TABLE_FAILURE_ALREADY_EXISTS,
    SYMBOL_TABLE_FAILURE_NOT_EXISTS
} SymbolTableResult;

/* Creates and initializes a new empty SymbolTable. Returns NULL if fails to create. */
SymbolTable* symbolTableCreate();

/* Destroys and frees the memory associated with the SymbolTable, including that of the Symbols stored in it. */
void symbolTableDestroy(SymbolTable *table);

/* Adds the given Symbol to the SymbolTable.
 * Returns SYMBOL_TABLE_FAILURE_ALREADY_EXISTS if a symbol with the same label already exists in the SymbolTable.
 * Returns SYMBOL_TABLE_SUCCESS upon success*/
SymbolTableResult symbolTableAddUnique(SymbolTable *table, Symbol *symbol);

/* Adds the given Symbol to the SymbolTable. Allows adding a symbols with the same label
 * Returns SYMBOL_TABLE_SUCCESS upon success*/
SymbolTableResult symbolTableAdd(SymbolTable *table, Symbol *symbol);

/* Retrieves a Symbol with the given label from the SymbolTable.
 * Returns SYMBOL_TABLE_FAILURE_NOT_EXISTS if no such symbol exist in the SymbolTable.
 * Returns SYMBOL_TABLE_SUCCESS upon success*/
SymbolTableResult symbolTableGet(SymbolTable *table, const char *label, Symbol **symbol);

#endif /* SYMBOL_TABLE_H */
