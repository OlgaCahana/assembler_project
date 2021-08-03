#include <stdio.h>
#include "first_pass.h"
#include "memory.h"
#include "parser.h"

/* Calculates the number of memory words required to encode the provided operand according to its addresing mode. */
int calcOperandMemory(Operand* operand){
    int wordsCount;

    if (operand == NULL) return 0;
    switch (operand->addressingMode){
        case IMMEDIATE_ADDRESSING:
            wordsCount = 1;
            break;

        case DIRECT_ADDRESSING:
            wordsCount = 1;
            break;

        case STRUCT_ADDRESSING:
            wordsCount = 2;
            break;

        case DIRECT_REGISTER_ADDRESSING:
            wordsCount=1;
            break;

        default:
            printf("Error at line %d! Undefined addressing mode.\n", lineCount);
            wordsCount=0;
        }
    return wordsCount;
    }



/* Calculates the number of memory words required to encode the provided instruction according to instruction type and its operands.*/
int calcInstructionMemory(Instruction* instruction){
    int wordsCount;
    wordsCount = calcOperandMemory(instruction->srcOperand) + calcOperandMemory(instruction->destOperand)+1;
    if(instruction->srcOperand != NULL && instruction->destOperand != NULL){
        if (instruction->srcOperand->addressingMode == DIRECT_REGISTER_ADDRESSING && instruction->destOperand->addressingMode == DIRECT_REGISTER_ADDRESSING){
          wordsCount--;
        }}
    return wordsCount;
}

/* Updates addresses in symbol table for data labels according to IC value
 * and the non data or extern labels according to the first instruction memory address (100 in our case), at the end of the first pass */
bool updateSymbolsAddr(SymbolTable *symbolTable){
    SymbolTableNode *node;

    if(symbolTable == NULL) return false;
    for (node = symbolTable->head; node !=NULL; node = node->next){
        if(node->symbol->type == SYMBOL_RELOCATABLE){
            node->symbol->addr+= IC + FIRST_MEMORY_LINE;
        } else if (node->symbol->type == SYMBOL_ABSOLUTE){
            node->symbol->addr+= FIRST_MEMORY_LINE;
        }
    }
    return true;
}

/* Receives a pointer to Line structure and apply the first pass algorithm.
 * Returns true if the algorithm completed with no errors and false otherwise. */
bool firstPassLine(Line* line, SymbolTable* symbolTable, unsigned short* dMemory){
    int i;
    Symbol *symbol = NULL;
    Symbol *symbolTemp;
    int temp;

    /* if the line contains a label declaration, create a new symbol.*/
    if(line->label != NULL){
        symbol = symbolCreate(line->label,0,SYMBOL_ABSOLUTE);
        if (symbol == NULL){
            printf("Error at line %d! Failed to allocate memory.\n", lineCount);
            return false;
        }
        /* update the new symbol according to line type (instruction/directive), and update if the symbol is extern or relocatable.*/
        if (line->instruction){
            symbol->addr = IC;
            symbol->type = SYMBOL_ABSOLUTE;
        } else if (line->directive){
            symbol->addr = DC;
            if(line->directive->directiveType == EXTERN)
                symbol->type = SYMBOL_EXTERNAL;
            else symbol->type = SYMBOL_RELOCATABLE;
        }
    }

    /* if the line is an instruction, calculate the number of memory words required for encoding.*/
    if (line->instruction) IC+=calcInstructionMemory(line->instruction);

    /* if the line is directive. proceed according to directive type. */
    if(line->directive) {
        switch (line->directive->directiveType) {
            /* if DATA, parse the data arguments and encode the values in data memory. */
            case DATA:
                while (intListDequeue(line->directive->dataParams, &temp)) {
                    dMemory[DC] = (unsigned short) temp;
                    DC++;
                }
                break;
            /* if STRING, parse the arguments and encode the values in data memory. */
            case STRING:
                for (i = 0; line->directive->stringParam[i] != '\0'; i++, DC++) {
                    dMemory[DC] = (unsigned short) (line->directive->stringParam[i]);
                }
                dMemory[DC] = '\0';
                DC++;
                break;
            /* if STRUCT, parse the arguments and encode the values in data memory. */
            case STRUCT:
                intListDequeue(line->directive->dataParams, &temp);
                dMemory[DC] = (unsigned short) temp;
                DC++;
                for (i = 0; line->directive->stringParam[i] != '\0'; i++, DC++) {
                    dMemory[DC] = (unsigned short) (line->directive->stringParam[i]);
                }
                dMemory[DC] = '\0';
                DC++;
                break;
            /* if EXTERN, create a new symbol for extern label */
            case EXTERN:
                symbol = symbolCreate(line->directive->labelParam, 0, SYMBOL_EXTERNAL);
                if (symbol == NULL) {
                    printf("Error at line %d! Failed to allocate memory.\n", lineCount);
                    return false;
                }
                break;
            /* if ENTRY, check that this label was not already declared as extern label */
            case ENTRY:
                if (symbolTableGet(symbolTable, line->directive->labelParam,&symbolTemp) == SYMBOL_TABLE_SUCCESS &&
                        symbolTemp->type == SYMBOL_EXTERNAL){
                    printf("Error at line %d! Label is already designated as external.\n", lineCount);
                    return false;
                }
                break;
            case INVALID_DIRECTIVE:
                return false;
        }
    }

    /* if a new symbol was created, add it to symbol table*/
    if (symbol != NULL) {
        switch (symbolTableAddUnique(symbolTable, symbol)) {
            case SYMBOL_TABLE_SUCCESS:
                break;
            case SYMBOL_TABLE_FAILURE_MEMORY_ALLOCATION:
                printf("Error at line %d! Symbol table is out of memory.\n", lineCount);
                return false;
                /* if the symbol exists, return error if not external, if external check if the one in the table is local
                 * if local, return error, if extern, ignore */
            case SYMBOL_TABLE_FAILURE_ALREADY_EXISTS:
                if (symbol->type != SYMBOL_EXTERNAL) {
                    printf("Error at line %d! This label was previously declared.\n", lineCount);
                    symbolDestroy(symbol);
                    return  false;
                } else {
                    symbolTableGet(symbolTable, symbol->label,&symbolTemp);
                    symbolDestroy(symbol);
                    if (symbolTemp->type != SYMBOL_EXTERNAL){
                        printf("Error at line %d! Local label cannot be declared as external.\n", lineCount);
                        return  false;
                    }
                    break;
                }
            default:
                return false;
        }
    }

    return true;

}

/* Reads the provided input file line by line.
 * Each line is parsed and then the information is stored.
 * All valid labels are stored in the symbol table and the directives data is encoded in data memory.
 * In case of error when parsing or encoding a line, an error is displayed to the user and next line is read.
 * The function returns true value if the entire file was read and encoded with no errors, otherwise it returns false.*/
bool firstPass(FILE* file, SymbolTable* symbolTable, unsigned short* dMemory ){
    char line[LINE_BUFFER];
    Line* newLine;  /* Line struct to store the parsed line information */
    bool retValue = true; /* store firstPassLine return value */
    bool errorFlag = false; /* flag raised if the retValue is false for one of the file lines */

    while(fgets(line,LINE_BUFFER,file) != NULL) {
        lineCount++;
        newLine = parseLine(line);
        if ((newLine != NULL) && (newLine->instruction !=NULL || newLine->directive != NULL)){
            retValue = firstPassLine(newLine, symbolTable, dMemory);
        }
        if (newLine == NULL || retValue == false){
            errorFlag = true;
        }
        lineDestroy(newLine);
    }

    /* if finished first pass with no errors, update the symbol table addresses according to IC value*/
    if(errorFlag == false){
        updateSymbolsAddr(symbolTable);
        fseek(file,0,SEEK_SET);
        return true;
    } else return false;
}