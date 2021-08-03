#ifndef ASSEMBLER_PROJECT_1_GLOBALS_H
#define ASSEMBLER_PROJECT_1_GLOBALS_H

#define MAX_LABEL_LENGTH 30
#define LINE_BUFFER 81
#define DECIMAL 10
#define MAX_MEMORY_SIZE  256
#define FIRST_MEMORY_LINE 100
#define MEMORY_SIZE 256

/* enum for possible directives */
enum directiveType {DATA, STRING, STRUCT, ENTRY, EXTERN, INVALID_DIRECTIVE};

/* enum for possible instructions */
enum instructionType {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, INVALID_INSTRUCTION};

/* enum for possible registers */
enum registerType {R0, R1, R2, R3, R4, R5, R6, R7, INVALID_REGISTER};

/* enum for possible addressing modes */
enum addressingMode {IMMEDIATE_ADDRESSING,DIRECT_ADDRESSING,STRUCT_ADDRESSING,DIRECT_REGISTER_ADDRESSING};

int lineCount; /*indicates the line number we're currently parsing*/
int IC; /* instructions counter */
int DC; /* directives counter */


#endif /* ASSEMBLER_PROJECT_1_GLOBALS_H */
