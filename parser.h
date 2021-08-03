#ifndef ASSEMBLER_PROJECT_1_PARSER_METHODS_H
#define ASSEMBLER_PROJECT_1_PARSER_METHODS_H

#include "structs.h"
#include <stdbool.h>

/* Skips tabs and spaces at the beginning of a string. Returns pointer to string suffix. */
char* skipWhiteSpaces(char* line );


/* Trim white spaces from the string tail. The provided string itself is modified, no return value. */
void trimTailWhitespaces(char* token);


/* Receives a line and gets the next white space separated token.
 * Returns a pointer to line suffix, i.e. next position after the token.
 * The token is stored in "token" variable which is passed as a parameter to this function.*/
char* getNextToken(char *line, char *token);


/* Receives a line and gets the next comma separated token.
 * Returns a pointer to line suffix, i.e. the next position after the token (the next position character is typically a comma or \n ).
 * The token is stored in "token" variable which is passed as a parameter to this function.*/
char* getNextCommaSeparatedToken(char *line, char *token);

/* Checks if there are any characters, which are not whitespaces, left in the line.
 * Returns true if such a characters were found.*/
bool searchForExtraCharacters(char *line);



/* Destroys and frees the memory associated with the instruction.*/
void instructionDestroy(Instruction* instruction);


/* Destroys and frees the memory associated with the directive.*/
void directiveDestroy(Directive* directive);


/* Destroys and frees the memory associated with the instruction.*/
void lineDestroy(Line *line);


/* Checks if a provided token is a valid instruction type.
 * Returns enum value according to the instruction type. */
enum instructionType isValidInstruction(char* token);


/* Checks if a provided token is a valid direction type.
 * Returns enum value according to the direction type. */
enum directiveType isValidDirective(char* token);


/* Checks if a provided token is a valid register name.
 * Returns enum value according to the register name. */
enum registerType isValidRegister(char* token);


/* The following function receives a pointer to Instruction and validates the addressing mode for Instruction source and destination operands.
 * It will return a false value if any of the provided addressing modes is illegal for this instruction type.
 * The information about legal addressing modes for each instruction is stored in instructionsDescript_table */
bool validateAddressingMode(Instruction* instruction);


/* Checks if the token is a valid label, without specific error messages.
 * Returns true if is valid label and false otherwise*/
bool isValidLabel(char *token);


/* Checks if the provided token is a reserved word: instruction name or directive name or register name. */
bool isReservedWord(char* token);



/*This function converts a string to an integer value.
 * Returns true upon success and false upon failure */
bool parseInteger(char* token, int* data);


/* Checks if the provided token is a valid label.
 * If valid label returns a pointer to the label, if not valid, prints an error and returns NULL. */
char* parseLabel(char *token);



/* Parses the data values. All the values are stored in integers linked list and a pointer to the list is returned.
 * If contains illegal data values or missing data values, prints an error and returns NULL */
IntList* parseData(char* line);



/* Receives a pointer to line and checks if this line contains a single valid string parameter.
 * A valid string parameter starts and ends with quotes (") and there is no extra characters after the second pair of quotes.
 * Returns a pointer to a valid string minus the quotes mark upon success and NULL upon failure. */
char* parseString(char *line);


/* Parses instruction operand. Upon success returns pointer to operand struct with appropriate operand value.
 * Upon failure returns NULL and prints appropriate error*/
Operand* parseOperand(char* token);


/* Parses a directive.
 * First checks if the directive name is valid, then parses the directive arguments according to directive type.
 * If errors are found, prints an error and returns NULL.
 * Upon success returns a pointer to directive struct.*/
Directive* parseDirective(char* token, char* line);


/* Parses an instruction.
 * First checks if the dinstruction name is valid, then parses the instruction operands according to instruction type.
 * If errors are found, prints an error and returns NULL.
 * Upon success returns a pointer to instruction struct.*/
Instruction* parseInstruction(char* token, char* line);


/* Parses input line.
 * Returns NULL if there's an error while parsing the line.
 * Returns pointer to Line struct with NULL fields if the line is a comment or a blank*/
Line* parseLine(char* line);


#endif /* ASSEMBLER_PROJECT_1_PARSER_METHODS_H */
