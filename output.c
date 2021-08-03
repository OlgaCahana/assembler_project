#include <stdio.h>
#include "output.h"
#include "weird32.h"
#include "globals.h"

/* Output file suffixes */
#define OBJECT_FILE_SUFFIX      "ob"
#define ENTRIES_FILE_SUFFIX     "ent"
#define EXTERNALS_FILE_SUFFIX   "ext"

/* Macro for calling fputs(), and returning cleanly in case of an error */
#define FPUTS_OR_RETURN(s) \
    if (fputs(s, file) == EOF) { \
        fclose(file); \
        return false; \
    }

/* Macro for calling fputc(), and returning cleanly in case of an error */
#define FPUTC_OR_RETURN(c) \
    if (fputc(c, file) == EOF) { \
        fclose(file); \
        return false; \
    }

/* Opens a file named <name>.<suffix> for reading or writing.
 * Returns NULL on error. */
FILE* openFile(const char *name, const char *suffix, const char *mode) {
    FILE *file;

    int nameLen = strlen(name);
    int suffixLen = strlen(suffix);

    char *filename = (char *)malloc((nameLen + 1 + suffixLen + 1) * sizeof(char));
    if (filename == NULL) {
        return NULL;
    }

    strcpy(filename,                name);
    strcpy(filename + nameLen,      ".");
    strcpy(filename + nameLen + 1,  suffix);

    file = fopen(filename, mode);
    free(filename);

    return file;
}

/* Writes a file in the format of the entries/externals files (lines of <label> <address>).
 * Returns true on success and false on error. */
bool writeSymbolsFile(const char *name, const char *suffix, SymbolTable* table) {
    FILE *file;
    char buf[8];
    SymbolTableNode *node;

    /* If there are no entries defined, skip creating output file. */
    if (table->head == NULL) {
        return true;
    }

    file = openFile(name, suffix, "w");
    if (file == NULL) {
        return false;
    }

    /* Iterate over all symbols, and write each one in a separate line */
    for (node = table->head; node != NULL; node = node->next) {
        FPUTS_OR_RETURN(node->symbol->label)
        FPUTC_OR_RETURN(' ')

        weirdEncodeNumber(node->symbol->addr, buf);
        FPUTS_OR_RETURN(buf)
        FPUTC_OR_RETURN('\n')
    }

    fclose(file);
    return true;
}

/* Writes an object file (*.ob) with the given instruction/data memory content.
 * Returns true on success and false on error. */
bool writeObjectFile(const char *name, unsigned short *iMemory, int ic, unsigned short *dMemory, int dc) {
    FILE *file;
    char buf[8];
    int i;

    file = openFile(name, OBJECT_FILE_SUFFIX, "w");
    if (file == NULL) {
        return false;
    }

    /* Write instruction and data memories lengths */
    weirdEncodeNumber(ic, buf);
    FPUTS_OR_RETURN(buf)
    FPUTC_OR_RETURN(' ')

    weirdEncodeNumber(dc, buf);
    FPUTS_OR_RETURN(buf)
    FPUTC_OR_RETURN('\n')

    /* Write instruction memory */
    for (i = 0; i < ic; i++) {
        weirdEncodeNumber(FIRST_MEMORY_LINE + i, buf);
        FPUTS_OR_RETURN(buf)
        FPUTC_OR_RETURN(' ')

        weirdEncodeWord(*(iMemory + i), buf);
        FPUTS_OR_RETURN(buf)
        FPUTC_OR_RETURN('\n')
    }

    /* Write instruction memory */
    for (i = 0; i < dc; i++) {
        weirdEncodeNumber(FIRST_MEMORY_LINE + ic + i, buf);
        FPUTS_OR_RETURN(buf)
        FPUTC_OR_RETURN(' ')

        weirdEncodeWord(*(dMemory + i), buf);
        FPUTS_OR_RETURN(buf)
        FPUTC_OR_RETURN('\n')
    }

    fclose(file);
    return true;
}

/* Writes an externals file (*.ext) with the given externals table.
 * Returns true on success and false on error. */
bool writeEntriesFile(const char *name, SymbolTable* entriesTable) {
    return writeSymbolsFile(name, ENTRIES_FILE_SUFFIX, entriesTable);
}

/* Writes an entries file (*.ent) with the given entries table.
 * Returns true on success and false on error. */
bool writeExternalsFile(const char *name, SymbolTable* externalsTable) {
    return writeSymbolsFile(name, EXTERNALS_FILE_SUFFIX, externalsTable);
}
