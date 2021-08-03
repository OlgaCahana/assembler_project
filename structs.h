#ifndef ASSEMBLER_PROJECT_1_STRUCTS_H
#define ASSEMBLER_PROJECT_1_STRUCTS_H

#include "globals.h"
#include "int_list.h"


/* Struct to hold operand data */
typedef struct _operand{
    char* lable; /* store regular or struct label, according to the addressing mode */
    int field;   /*store the register number or immediate field value or struct field number, according to the addressing mode */
    enum addressingMode addressingMode;
} Operand;


/* Instruction struct. Holds the instruction type and pointers to source and destination operands */
typedef struct _instruction{
    enum instructionType instructionType;
    Operand* srcOperand;
    Operand* destOperand;

} Instruction;


/* Directive struct. Holds the directive type and pointers to other parameters */
typedef struct _directive{
    enum directiveType directiveType;
    IntList* dataParams; /* linked list to store "data" directive int arguments, or a single int value for "struct" directive */
    char* stringParam; /* a string to store the "string" directive argument or the string belonging to "struct" directive field */
    char* labelParam; /* store label name for "entry" and "extern" directives */

} Directive;

/* Line struct. Holds the label (optional) and pointers to directive and instruction structs, one of those two pointers have to be NULL*/
typedef struct _line{
    char* label;
    Instruction* instruction;
    Directive* directive;
} Line;

/* Struct to hold the instruction information, regarding expected operands and allowed addressing modes  */
typedef struct instructionDescript{
    bool srcOperand;    /* 1 - src operand required, 0 - not required*/
    bool src_allowedAddressingModes[4]; /*four fields for each addressing mode, 1 - if legal for src operand, 0 - if illegal*/
    bool destOperand;   /* 1- destination operand required, 0 - not required*/
    bool dest_allowedAddressingModes[4]; /*four fields for each addressing mode, 1 - if legal for dest operand, 0 - if illegal*/

} InstructionDescript;


#endif /* ASSEMBLER_PROJECT_1_STRUCTS_H */
