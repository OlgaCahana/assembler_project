#ifndef ASSEMBLER_PROJECT_1_COMPILE_FILE_H
#define ASSEMBLER_PROJECT_1_COMPILE_FILE_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include "structs.h"
#include "parser.h"
#include "globals.h"
#include "first_pass.h"
#include "second_pass.h"
#include "symbol_table.h"
#include "memory.h"
#include "output.h"

/* defines the possible return values for compileFile function */
typedef enum {
    COMPILE_SUCCESS,
    COMPILE_FAILURE_MEMORY_ALLOCATION,
    COMPILE_FAILURE_FIRST_PASS,
    COMPILE_FAILURE_SECOND_PASS,
    COMPILE_FAILURE_READ_FILE,
    COMPILE_FAILURE_WRITE_TO_FILE
} CompileResult;

/* This function processes a single file.
 * It creates the symbol table and the memory image for provided file.
 * If error occurred at any step in the process (opening the file, first pass, second pass, output generation),
 * appropriate error type is returned. Upon success returns COMPILE_SUCCESS*/
CompileResult compileFile(const char *name);

#endif /* ASSEMBLER_PROJECT_1_COMPILE_FILE_H */
