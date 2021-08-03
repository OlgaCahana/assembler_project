#include <stdio.h>
#include "second_pass.h"
#include "parser.h"
#include "symbol_table.h"
#include "output.h"



/* Handles the directives of ENTRY type.
 * Checks if the entry label is defined as a local label, if undefined or defined as extern, appropriate error is returned.
 * If defined correctly, the label is added to the entry table as a new symbol. */
bool handleDirective(Directive* directive, SymbolTable* symbolTable, SymbolTable* entryTable) {
    Symbol *symbol;
    Symbol *newSymbol;

    if (directive->directiveType == ENTRY) {
        if (symbolTableGet(symbolTable, directive->labelParam, &symbol) == SYMBOL_TABLE_FAILURE_NOT_EXISTS) {
            printf("Error at line %d! Undefined label.\n", lineCount);
            return false;
        } else if (symbol->type == SYMBOL_EXTERNAL){
            printf("Error at line %d! External label cannot be declared as entry.\n", lineCount);
            return false;
        }
        newSymbol = symbolCreate(symbol->label,symbol->addr,symbol->type);
        if (newSymbol == NULL){
            printf("Error at line %d! Failed to allocate memory.\n", lineCount);
            return false;
        }
        if (symbolTableAddUnique(entryTable, newSymbol) == SYMBOL_TABLE_FAILURE_MEMORY_ALLOCATION) {
            printf("Error at line %d! Failed to allocate memory.\n", lineCount);
            return false;
        }
    }
        return true;
    }



/* Encodes label address and ARE bits in additional memory word, according to the information in the symbol table.
 * If the label is extern, updates the extern symbols table. */
bool encodeLabel(char* label, SymbolTable* symbolTable, SymbolTable* externTable, unsigned short* iMemory){
    Symbol *symbol;
    Symbol* extSymbol; /* external symbol to add to externTable if needed */
    if (symbolTableGet(symbolTable, label, &symbol) == SYMBOL_TABLE_FAILURE_NOT_EXISTS) {
        printf("Error at line %d! Undefined label.\n", lineCount);
        return false;
    }
    iMemory[IC] = (unsigned short)symbol->addr;
    iMemory[IC]<<= 2;

    if(symbol->type == SYMBOL_EXTERNAL){
        extSymbol = symbolCreate(label,IC + FIRST_MEMORY_LINE,SYMBOL_EXTERNAL);
        if (extSymbol == NULL){
            printf("Error at line %d! Failed to allocate memory.\n", lineCount);
            return false;
        }
        if (symbolTableAdd(externTable, extSymbol) == SYMBOL_TABLE_FAILURE_MEMORY_ALLOCATION) {
            printf("Error at line %d! Failed to allocate memory.\n", lineCount);
            return false;
        }
        iMemory[IC] |= SYMBOL_EXTERNAL;
    } else iMemory[IC] |= SYMBOL_RELOCATABLE;
    return true;
}



/* Receives an operand and encodes one or two additional memory words according to its addressing mode.
 * If label encoding is required (in case of direct or struct addressing), the encode label is called.
 * If the label is external it will be added to extern table with the line address it appears.*/
bool encodeOperand(Operand* operand, bool isSrc, SymbolTable* symbolTable, SymbolTable* externTable, unsigned short* iMemory ){

    switch (operand->addressingMode) {
        case IMMEDIATE_ADDRESSING:
            iMemory[IC] = (unsigned short) operand->field;
            iMemory[IC] <<= 2;
            break;
        case DIRECT_REGISTER_ADDRESSING:
            iMemory[IC] = (unsigned short) operand->field;
            iMemory[IC] <<= isSrc ? 6 : 2;
            break;
        case DIRECT_ADDRESSING:
            if (encodeLabel(operand->lable, symbolTable, externTable, iMemory) == false)
                return false;
            else break;
        case STRUCT_ADDRESSING:
            if (encodeLabel(operand->lable, symbolTable, externTable, iMemory) == false)
                return false;
            iMemory[++IC] = (unsigned short)operand->field;
            iMemory[IC] <<= 2;
            iMemory[IC] |= SYMBOL_ABSOLUTE;
            break;
    }
    IC++;
    return true;
}



/* Encodes the instruction first memory word and the additional memory words according to operands type. */
bool encodeInstruction(Instruction* instruction, SymbolTable* symbolTable, SymbolTable* externTable, unsigned short* iMemory){
    unsigned short word =0;

    /* encode opcode word */
    word |= instruction->instructionType;
    word <<= 2;
    if (instruction->srcOperand) {
        word |= instruction->srcOperand->addressingMode;
    }
    word <<= 2;
    if (instruction->destOperand){
        word |= instruction->destOperand->addressingMode;
    }
    word <<= 2;
    word |= SYMBOL_ABSOLUTE;

    iMemory[IC++] = word;

    /* if src and dest operands are both registers, encode only one additional word, else encode each operand by itself*/
    word = 0;
    if(instruction->srcOperand != NULL &&  instruction->srcOperand->addressingMode == DIRECT_REGISTER_ADDRESSING &&
            instruction->destOperand != NULL && instruction->destOperand->addressingMode == DIRECT_REGISTER_ADDRESSING){
        word |= instruction->srcOperand->field;
        word <<= 4;
        word |= instruction->destOperand->field;
        word <<=2;
        iMemory[IC++] = word;
    } else{
        if (instruction->srcOperand != NULL) {
            encodeOperand(instruction->srcOperand, true, symbolTable, externTable, iMemory);
        }
        if (instruction->destOperand != NULL) {
            encodeOperand(instruction->destOperand, false, symbolTable, externTable, iMemory);
        }
    }
    return true;
}



/* Receives a pointer to Line structure and calls the encoding function according to line type: instruction or directive.
 * Returns true if the called function returns true as well, and false otherwise. */
bool secondPassLine(Line* line, SymbolTable* symbolTable, SymbolTable* entryTable, SymbolTable* externTable, unsigned short* iMemory) {

    if (line->directive != NULL) {
        if(handleDirective(line->directive, symbolTable, entryTable) == false) {
            return false;
        }
    }

    if (line->instruction != NULL){
        if(encodeInstruction(line->instruction, symbolTable, externTable, iMemory) == false) {
            return false;
        }
    }

    return true;
}


/* Reads the provided input file line by line.
 * Each line is parsed and then the information is stored.
 * Encodes the required memory words for each line in the file (data or instructions memory) .
 * In case of error when parsing or encoding a line, an error is displayed to the user and next line is read.
 * The function returns true value if the entire file was read and encoded with no errors, otherwise it returns false.*/
bool secondPass(FILE* file, SymbolTable* symbolTable, SymbolTable* entryTable, SymbolTable* externTable, unsigned short* iMemory, unsigned short* dMemory ){
    char line[LINE_BUFFER];
    Line* newLine; /* Line struct to store the parsed line information */
    bool retValue = true; /* store secondPassLine return value */
    bool errorFlag = false; /* flag raised if the retValue is false for one of the file lines */

    IC = 0;
    lineCount = 0;

    if (entryTable == NULL || externTable == NULL) {
        printf("Error at line %d! Failed to allocate memory.\n", lineCount);
        return false;
    }

    while(fgets(line,LINE_BUFFER,file) != NULL) {
        lineCount++;
        newLine = parseLine(line);
        if ((newLine != NULL) && (newLine->instruction !=NULL || newLine->directive != NULL)){
            retValue = secondPassLine(newLine, symbolTable, entryTable, externTable, iMemory);
        }
        if (newLine == NULL || retValue == false){
            errorFlag = true;
        }
        lineDestroy(newLine);
    }

    if (errorFlag == true) {
        return false;
    }
    return true;

}

