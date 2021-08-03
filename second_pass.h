#ifndef ASSEMBLER_PROJECT_1_SECOND_PASS_H
#define ASSEMBLER_PROJECT_1_SECOND_PASS_H

#include <stdio.h>
#include "structs.h"
#include "symbol_table.h"


/* Handles the directives of ENTRY type.
 * Checks if the entry label is defined as a local label, if undefined or defined as extern, appropriate error is returned.
 * If defined correctly, the label is added to the entry table as a new symbol. */
bool handleDirective(Directive* directive, SymbolTable* symbolTable, SymbolTable* entryTable);


/* Encodes label address and ARE bits in additional memory word, according to the information in the symbol table.
 * If the label is extern, updates the extern symbols table. */
bool encodeLabel(char* label, SymbolTable* symbolTable, SymbolTable* externTable, unsigned short* iMemory);


/* Receives an operand and encodes one or two additional memory words according to its addressing mode.
 * If label encoding is required (in case of direct or struct addressing), the encode label is called.
 * If the label is external it will be added to extern table with the line address it appears.*/
bool encodeOperand(Operand* operand, bool isSrc, SymbolTable* symbolTable, SymbolTable* externTable, unsigned short* iMemory );


/* Encodes the instruction first memory word and the additional memory words according to operands type. */
bool encodeInstruction(Instruction* instruction, SymbolTable* symbolTable, SymbolTable* externTable, unsigned short* iMemory);


/* Receives a pointer to Line structure and calls the encoding function according to line type: instruction or directive.
 * Returns true if the called function returns true as well, and false otherwise. */
bool secondPassLine(Line* line, SymbolTable* symbolTable, SymbolTable* entryTable, SymbolTable* externTable, unsigned short* iMemory);


/* Reads the provided input file line by line.
 * Each line is parsed and then the information is stored.
 * Encodes the required memory words for each line in the file (data or instructions memory) .
 * In case of error when parsing or encoding a line, an error is displayed to the user and next line is read.
 * The function returns true value if the entire file was read and encoded with no errors, otherwise it returns false.*/
bool secondPass(FILE* file, SymbolTable* symbolTable, SymbolTable* entryTable, SymbolTable* externTable, unsigned short* iMemory, unsigned short* dMemory );

#endif /* ASSEMBLER_PROJECT_1_SECOND_PASS_H */
