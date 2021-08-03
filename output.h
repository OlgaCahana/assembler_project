#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

#ifndef OUTPUT_H
#define OUTPUT_H

/* Writes an object file (*.ob) with the given instruction/data memory content.
 * Returns true on success and false on error. */
bool writeObjectFile(const char *name, unsigned short *iMemory, int ic, unsigned short *dMemory, int dc);

/* Writes an entries file (*.ent) with the given entries table.
 * Returns true on success and false on error. */
bool writeEntriesFile(const char *name, SymbolTable* entriesTable);

/* Writes an externals file (*.ext) with the given externals table.
 * Returns true on success and false on error. */
bool writeExternalsFile(const char *name, SymbolTable* externalsTable);

/* Opens a file named <name>.<suffix> for reading or writing.
 * Returns NULL on error. */
FILE* openFile(const char *name, const char *suffix, const char *mode);

#endif /* OUTPUT_H */
