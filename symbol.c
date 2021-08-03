#include <stdlib.h>
#include <string.h>
#include "symbol.h"

/* Creates and initializes a new Symbol from the given arguments.*/
Symbol* symbolCreate(const char *label, int addr, SymbolEncodingType type) {
    Symbol *symbol;

    if (label == NULL) {
        return NULL;
    }

    symbol = (Symbol*)malloc(sizeof(struct symbol));
    if (symbol == NULL) {
        return NULL;
    }

    symbol->label = (char *)malloc((strlen(label) + 1) * sizeof(char));
    if (symbol->label == NULL) {
        free(symbol);
        return NULL;
    }
    strcpy(symbol->label, label);

    symbol->addr = addr;
    symbol->type = type;

    return symbol;
}

/* Destroys and frees the memory associated with the Symbol.*/
void symbolDestroy(Symbol *symbol) {
    if (symbol == NULL) {
        return;
    }
    free(symbol->label);
    free(symbol);

}
