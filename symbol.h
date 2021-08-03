#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>

/* Possible encoding types for a symbol */
typedef enum {
    SYMBOL_ABSOLUTE,
    SYMBOL_EXTERNAL,
    SYMBOL_RELOCATABLE
} SymbolEncodingType;

/* A symbol (label) in the assembly program */
typedef struct symbol {
    char *label;
    int addr;
    SymbolEncodingType type;
} Symbol;

/* Creates and initializes a new Symbol from the given arguments.*/
Symbol* symbolCreate(const char *label, int addr, SymbolEncodingType type);

/* Destroys and frees the memory associated with the Symbol.*/
void symbolDestroy(Symbol *symbol);

#endif /* SYMBOL_H */
