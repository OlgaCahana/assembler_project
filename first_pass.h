#ifndef ASSEMBLER_PROJECT_1_FIRST_PASS_H
#define ASSEMBLER_PROJECT_1_FIRST_PASS_H

#include <stdio.h>
#include "structs.h"
#include "symbol_table.h"


/* Calculates the number of memory words required to encode the provided operand according to its addresing mode. */
int calcOperandMemory(Operand* operand);


/* Calculates the number of memory words required to encode the provided instruction according to instruction type and its operands.*/
int calcInstructionMemory(Instruction* instruction);


/* Updates addresses in symbol table for data labels according to IC value
 * and the non data or extern labels according to the first instruction memory address (100 in our case), at the end of the first pass */
bool updateSymbolsAddr(SymbolTable *symbolTable);


/* Receives a pointer to Line structure and apply the first pass algorithm.
 * Returns true if the algorithm completed with no errors and false otherwise. */
bool firstPassLine(Line* line, SymbolTable* symbolTable, unsigned short* dMemory);


/* Reads the provided input file line by line.
 * Each line is parsed and then the information is stored.
 * All valid labels are stored in the symbol table and the directives data is encoded in data memory.
 * In case of error when parsing or encoding a line, an error is displayed to the user and next line is read.
 * The function returns true value if the entire file was read and encoded with no errors, otherwise it returns false.*/
bool firstPass(FILE* file, SymbolTable* symbolTable, unsigned short* dMemory );



#endif /* ASSEMBLER_PROJECT_1_FIRST_PASS_H */
