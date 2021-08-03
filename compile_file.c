#include "compile_file.h"

/* This function processes a single file.
 * It creates the symbol table and the memory image for provided file.
 * If error occurred at any step in the process (opening the file, first pass, second pass, output generation),
 * appropriate error type is returned. Upon success returns COMPILE_SUCCESS*/
CompileResult compileFile(const char *name){
    SymbolTable* symbolTable; /* store the file Symbols table*/
    SymbolTable* entryTable;    /* store the file entries  - list of labels declared as entries and their address in symbol table */
    SymbolTable* externTable;   /* store the file externs  - list of labels declared as extern and line address where used */
    unsigned short int iMemory[MAX_MEMORY_SIZE] = {0}; /* code memory image */
    unsigned short int dMemory[MAX_MEMORY_SIZE] = {0}; /* data memory image */
    FILE *file;

    file = openFile(name, "as", "r");
    if (file == NULL){
        return COMPILE_FAILURE_READ_FILE;
    }
    /* allocate symbols, entry and extern tables */
    symbolTable = symbolTableCreate();
    entryTable = symbolTableCreate();
    externTable = symbolTableCreate();

    if (symbolTable == NULL || entryTable == NULL || externTable == NULL){
        fclose(file);
        symbolTableDestroy(symbolTable);
        symbolTableDestroy(entryTable);
        symbolTableDestroy(externTable);
        return COMPILE_FAILURE_MEMORY_ALLOCATION;
    }

    /* run first pass on input file, if errors found return COMPILE_FAILURE_FIRST_PASS */
    if(firstPass(file, symbolTable, dMemory) == false) {
        fclose(file);
        symbolTableDestroy(symbolTable);
        symbolTableDestroy(entryTable);
        symbolTableDestroy(externTable);
        return COMPILE_FAILURE_FIRST_PASS;
    }
    /* run second pass on input file, if errors found return COMPILE_FAILURE_SECOND_PASS */
    if(secondPass(file, symbolTable, entryTable, externTable,iMemory, dMemory) == false){
        fclose(file);
        symbolTableDestroy(symbolTable);
        symbolTableDestroy(entryTable);
        symbolTableDestroy(externTable);
        return COMPILE_FAILURE_SECOND_PASS;
    }
    /* cretae output files */
    if (writeEntriesFile(name, entryTable) == false ||
        writeExternalsFile(name, externTable) == false ||
        writeObjectFile(name, iMemory, IC, dMemory, DC ) == false){
        fclose(file);
        symbolTableDestroy(symbolTable);
        symbolTableDestroy(entryTable);
        symbolTableDestroy(externTable);
        return COMPILE_FAILURE_WRITE_TO_FILE;
    }

    /* close the file and free memory */
    fclose(file);
    symbolTableDestroy(symbolTable);
    symbolTableDestroy(entryTable);
    symbolTableDestroy(externTable);

    /* check for memory overflow */
    if(IC + DC > MEMORY_SIZE){
        printf("Warning! Memory overflow! cannot encode more than %d addresses.\n", MEMORY_SIZE);
    }

    return COMPILE_SUCCESS;
}
