#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

/* The information about legal addressing modes for each instruction operands is stored in instructionsDescript_table.
 * The row index indicates the instruction type, according to instruction opcode.
 * Each row is a struct that describes if source or destination operand is required for this specific instruction,
 * and then the legal addressing modes are listed for each operand:
 * 1 - if legal, 0 - if illegal/*/
InstructionDescript instructionsDescript_table[16] = {
                                                        {1,{1,1,1,1},1,{0,1,1,1}},  /*mov*/
                                                        {1,{1,1,1,1},1,{1,1,1,1}},  /*cmp*/
                                                        {1,{1,1,1,1},1,{0,1,1,1}},  /*add*/
                                                        {1,{1,1,1,1},1,{0,1,1,1}},  /*sub*/
                                                        {0,{0,0,0,0},1,{0,1,1,1}},  /*not*/
                                                        {0,{0,0,0,0},1,{0,1,1,1}},  /*clr*/
                                                        {1,{0,1,1,0},1,{0,1,1,1}},  /*lea*/
                                                        {0,{0,0,0,0},1,{0,1,1,1}},  /*inc*/
                                                        {0,{0,0,0,0},1,{0,1,1,1}},  /*dec*/
                                                        {0,{0,0,0,0},1,{0,1,1,1}},  /*jmp*/
                                                        {0,{0,0,0,0},1,{0,1,1,1}},  /*bne*/
                                                        {0,{0,0,0,0},1,{0,1,1,1}},  /*red*/
                                                        {0,{0,0,0,0},1,{1,1,1,1}},  /*prn*/
                                                        {0,{0,0,0,0},1,{0,1,1,1}},  /*jsr*/
                                                        {0,{0,0,0,0},0,{0,0,0,0}},  /*rts*/
                                                        {0,{0,0,0,0},0,{0,0,0,0}}   /*stop*/
                                                    };

/* Skips tabs and spaces at the beginning of a string. Returns pointer to string suffix. */
char* skipWhiteSpaces(char* line ){
    while(*line != '\0'){
        if(*line == ' ' || *line == '\t') {
            line++;
            continue;
        }
        else break;
    }
    return line;
}



/* Trim white spaces from the string tail. The provided string itself is modified, no return value. */
void trimTailWhitespaces(char* token){
    int lastIndex = strlen(token);
    while (lastIndex > 0 && isspace((unsigned char)(token[lastIndex-1]))){
       lastIndex--;
    }
    token[lastIndex] = '\0';
}



/* Receives a line and gets the next white space separated token.
 * Returns a pointer to line suffix, i.e. next position after the token.
 * The token is stored in "token" variable which is passed as a parameter to this function.*/
char* getNextToken(char *line, char *token){
    int i = 0;
    line = skipWhiteSpaces(line);
    while(*line != ' ' && *line != '\t' && *line != '\0' && *line != '\n'){
        token[i] = *line;
        i++;
        line++;
    }
    token[i] = '\0';
    return line;
}



/* Receives a line and gets the next comma separated token.
 * Returns a pointer to line suffix, i.e. the next position after the token (the next position character is typically a comma or \n ).
 * The token is stored in "token" variable which is passed as a parameter to this function.*/
char* getNextCommaSeparatedToken(char *line, char *token){
    int i = 0;
    line = skipWhiteSpaces(line);
    while(*line != '\n' && *line != ',' && *line !='\0'){
        token[i] = *line;
        i++;
        line++;
    }
    token[i] = '\0';
    trimTailWhitespaces(token);
    return line;
}



/* Checks if there are any characters, which are not whitespaces, left in the line.
 * Returns true if such a characters were found.*/
bool searchForExtraCharacters(char *line){
    char token[LINE_BUFFER];
    getNextToken(line, token);
    if(strlen(token)) {
        printf("Error at line %d! Excessive characters.\n", lineCount);
        return true;
    } else return false;

}



/* Destroys and frees the memory associated with the instruction.*/
void lineDestroy(Line *line){
    if(line == NULL) {
        return;
    }
    free(line->label);
    instructionDestroy(line->instruction);
    directiveDestroy(line->directive);
    free(line);
}



/* Destroys and frees the memory associated with the instruction.*/
void instructionDestroy(Instruction *instruction){
    if(instruction == NULL) {
        return;
    }
    if (instruction->srcOperand != NULL){
        free(instruction->srcOperand->lable);
        free(instruction->srcOperand);
    }
    if (instruction->destOperand != NULL){
        free(instruction->destOperand->lable);
        free(instruction->destOperand);
    }
    free(instruction);
}



/* Destroys and frees the memory associated with the directive.*/
void directiveDestroy(Directive *directive){
    if(directive == NULL) {
        return;
    }
    intListDestroy(directive->dataParams);
    free(directive->labelParam);
    free(directive->stringParam);
    free(directive);
}



/* Checks if a provided token is a valid instruction type.
 * Returns enum value according to the instruction type. */
enum instructionType isValidInstruction(char *token){
    if(strcmp(token, "mov") == 0) return MOV;
    else if (strcmp(token, "cmp") == 0) return CMP;
    else if (strcmp(token, "add") == 0) return ADD;
    else if (strcmp(token, "sub") == 0) return SUB;
    else if (strcmp(token, "not") == 0) return NOT;
    else if (strcmp(token, "clr") == 0) return CLR;
    else if (strcmp(token, "lea") == 0) return LEA;
    else if (strcmp(token, "inc") == 0) return INC;
    else if (strcmp(token, "dec") == 0) return DEC;
    else if (strcmp(token, "jmp") == 0) return JMP;
    else if (strcmp(token, "bne") == 0) return BNE;
    else if (strcmp(token, "red") == 0) return RED;
    else if (strcmp(token, "prn") == 0) return PRN;
    else if (strcmp(token, "jsr") == 0) return JSR;
    else if (strcmp(token, "rts") == 0) return RTS;
    else if (strcmp(token, "stop") == 0) return STOP;
    else return INVALID_INSTRUCTION;
}



/* Checks if a provided token is a valid register name.
 * Returns enum value according to the register name. */
enum registerType isValidRegister(char *token){
    if (strcmp(token,"r0") == 0) return R0;
    if (strcmp(token,"r1") == 0) return R1;
    if (strcmp(token,"r2") == 0) return R2;
    if (strcmp(token,"r3") == 0) return R3;
    if (strcmp(token,"r4") == 0) return R4;
    if (strcmp(token,"r5") == 0) return R5;
    if (strcmp(token,"r6") == 0) return R6;
    if (strcmp(token,"r7") == 0) return R7;
    else return INVALID_REGISTER;
}



/* Checks if a provided token is a valid direction type.
 * Returns enum value according to the direction type. */
enum directiveType isValidDirective(char *token){
    if (strcmp(token,"data") == 0) return DATA;
    if (strcmp(token,"string") == 0) return STRING;
    if (strcmp(token,"struct") == 0) return STRUCT;
    if (strcmp(token,"entry") == 0) return ENTRY;
    if (strcmp(token,"extern") == 0) return EXTERN;
    else return INVALID_DIRECTIVE;
}



/* The following function receives a pointer to Instruction and validates the addressing mode for Instruction source and destination operands.
 * It will return a false value if any of the provided addressing modes is illegal for this instruction type.
 * The information about legal addressing modes for each instruction is stored in instructionsDescript_table */
bool validateAddressingMode(Instruction *instruction){

    /* validate source operand addressing mode */
    if(instructionsDescript_table[instruction->instructionType].srcOperand == 1){
        if(instructionsDescript_table[instruction->instructionType].src_allowedAddressingModes[instruction->srcOperand->addressingMode] != 1){
            printf("Error at line %d! Source operand addressing mode is illegal for this instruction type.\n", lineCount);
            return false;
        }
    }
    /* validate destination operand addressing mode */
    if(instructionsDescript_table[instruction->instructionType].destOperand == 1){
        if(instructionsDescript_table[instruction->instructionType].dest_allowedAddressingModes[instruction->destOperand->addressingMode] != 1){
            printf("Error at line %d! Destination operand addressing mode is illegal for this instruction type.\n", lineCount);
            return false;
        }
    }
    return true;
}



/* Checks if the token is a valid label, without specific error messages.
 * Returns true if is valid label and false otherwise*/
bool isValidLabel(char *token){
    int i = 1;

    if (isalpha(token[0]) == 0){
        return  false;
    }
    while (token[i] != '\0') {
        if (i > MAX_LABEL_LENGTH) return false;

        if (isalnum(token[i])) {
            i++;
        }
        else return false;
    }
    if(isReservedWord(token) == true) return false;

    return true;
}



/* Checks if the provided token is a reserved word: instruction name or directive name or register name. */
bool isReservedWord(char *token){
    if(isValidInstruction(token) == INVALID_INSTRUCTION &&
            isValidDirective(token) == INVALID_DIRECTIVE &&
            isValidRegister(token) == INVALID_REGISTER)
        return false;
    else return true;
}



/* Checks if the provided token is a valid label.
 * If valid label returns a pointer to the label, if not valid, prints an error and returns NULL. */
char* parseLabel(char *token){
    int i = 0;
    char* label;

    if (*token == '\0') {
        printf("Error at line %d! Label name cannot be empty.\n", lineCount);
        return NULL;
    }
    if (isalpha(token[i]) == 0){
        printf("Error at line %d! Label has non alphabetic first character.\n", lineCount);
        return  NULL;
    }

    while (token[i] != '\0') {
        if (i > MAX_LABEL_LENGTH) {
            printf("Error at line %d! Label name is too long.\n", lineCount);
            return NULL;
        }

        if (isalnum(token[i])) {
            i++;
            continue;
        }
        else {
            printf("Error at line %d! Label name contains non alphanumeric characters.\n", lineCount);
            return NULL;
        }
    }

    if(isReservedWord(token)){
        printf("Error at line %d! A reserved word cannot be used as a label.\n", lineCount);
        return NULL;
    }

    label = (char *)malloc((i+1)*sizeof(char));
    if(label == NULL){
        printf("Error at line %d! Failed to allocate memory.\n", lineCount);
        return NULL;
    }
    strncpy(label, token, i);
    label[i] = '\0';

    return label;
}



/*This function converts a string to an integer value.
 * Returns true upon success and false upon failure */
bool parseInteger(char *token, int *data){
    char* end;
    if(strlen(token) == 0) return false;
    *data = strtol(token, &end, DECIMAL);
    if (*end != '\0') return false;
    else return true;

}



/* Parses the data values. All the values are stored in integers linked list and a pointer to the list is returned.
 * If contains illegal data values or missing data values, prints an error and returns NULL */
IntList* parseData(char *line){
    IntList* dataList;
    int dataElement;
    char token[LINE_BUFFER];
    bool expectElement = true;

    dataList = intListCreate();
    if (dataList == NULL){
        printf("Error at line %d! Failed to allocate memory.\n", lineCount);
        return NULL;
    }

    if (*line == '\0'){
        printf("Error at line %d! Missing data element.\n", lineCount);
        intListDestroy(dataList);
        return NULL;
    }
    while(*line != '\0'){
        line = getNextCommaSeparatedToken(line,token);
        if (expectElement == true && *token == '\0'){
            printf("Error at line %d! Missing data element..\n", lineCount);
            intListDestroy(dataList);
            return NULL;
        }

        if (!parseInteger(token,&dataElement)){
            printf("Error at line %d! Data element is not an integer.\n", lineCount);
            intListDestroy(dataList);
            return NULL;
        }
        if (dataElement > 511 || dataElement < -512){
            printf("Error at line %d! Data element value exceeds the limits. i.e. is too large or too small to fit 10 bytes\n", lineCount);
            intListDestroy(dataList);
            return NULL;
        }
        if (intListEnqueue(dataList,dataElement) == false){
            printf("Error at line %d! Failed to store data element.\n", lineCount);
            intListDestroy(dataList);
            return NULL;
        };

        if (*line == '\n') expectElement = false;
        if (*line != '\0') line++; /* skip the comma or "\n" */
    }
    return dataList;
}



/* Receives a pointer to line and checks if this line contains a single valid string parameter.
 * A valid string parameter starts and ends with quotes (") and there is no extra characters after the second pair of quotes.
 * Returns a pointer to a valid string minus the quotes mark upon success and NULL upon failure. */
char* parseString(char *line){
    char *string = NULL;
    char *iterator = NULL;
    bool errFlag = false;

    line = skipWhiteSpaces(line);
    if (*line == '\0' || *line == '\n'){
        printf("Error at line %d! Missing string argument in directive.\n", lineCount);
        return NULL;
    }
    if (*line != '"'){
        errFlag = true;
    } else {
        iterator = ++line;
        while (*iterator != '"') {
            if (*iterator == '\0' || *iterator == '\n') {
                errFlag = true;
                break;
            }
            iterator++;
        }
           if (searchForExtraCharacters(iterator+1)){
               return NULL;
         }
    }

    /* if error flag is up, the string is illegal*/
    if(errFlag){
        printf("Error at line %d! Illegal string in directive.\n", lineCount);
        return NULL;
    }

    /* if reached here, the string is legal, allocate memory to store the string object and return pointer to it */
    string = (char*)malloc((iterator-line+1)* sizeof(char));
    if(string == NULL){
        printf("Error at line %d! Failed to allocate memory.\n", lineCount);
        return NULL;
    }
    *iterator = '\0';
    strcpy(string, line);
    return string;
}



/* Parses instruction operand. Upon success returns pointer to operand struct with appropriate operand value.
 * Upon failure returns NULL and prints appropriate error*/
Operand* parseOperand(char *token){
    Operand* newOperand;
    int status;
    char structLabel[MAX_LABEL_LENGTH+1] = {0};
    int data;
    int i=0;
    int j = 0;


    newOperand = (Operand*)malloc(sizeof(Operand));
    if(newOperand == NULL){
        printf("Error at line %d! Failed to allocate memory.\n", lineCount);
        return NULL;
    }
    newOperand->lable = NULL;

/* parsing if immediate operand */
    if(token[i] == '#') {
        token++;

        if (!parseInteger(token, &data)) {
            printf("Error at line %d! Failed to parse an integer value.\n", lineCount);
            free(newOperand);
            return NULL;
        }

        newOperand->field = data;
        if (newOperand->field > 127 || newOperand->field < -128) {
            printf("Error at line %d! Immediate operand value exceeds the limits. i.e too large or too small to fit 8 bytes.\n", lineCount);
            free(newOperand);
            return NULL;
        }
        newOperand->addressingMode = IMMEDIATE_ADDRESSING;
        newOperand->lable = NULL;
        return newOperand;
    }

/* parsing if register operand */
    status = isValidRegister(token);
    if(status != INVALID_REGISTER){
        newOperand->field = status;
        newOperand->addressingMode = DIRECT_REGISTER_ADDRESSING;
        newOperand->lable = NULL;
        return newOperand;
    }

/* parsing if label operand */
    status = isValidLabel(token);
    if(status){
        newOperand->field = 0;
        newOperand->addressingMode = DIRECT_ADDRESSING;
        newOperand->lable = (char*)malloc((strlen(token)+1)* sizeof(char));
        if(newOperand->lable == NULL){
            printf("Error at line %d! Failed to allocate memory.\n", lineCount);
            return NULL;
        }
        strcpy(newOperand->lable,token);
        return newOperand;
    }

/* parsing if struct operand */
    while(token[i] != '\0'){
        if(token[i] == '.') break;
        i++;
    }
    /* if dot is found, dividing into two tokens: structLabel to store the label , and token to store the struct field */
    if(token[i] == '.'){
        for (j = 0; j<i;j++){
            structLabel[j] = token[0];
            token++;
        }
        structLabel[j] = '\0';
        token++;
    }
    status = isValidLabel(structLabel);
    if (status && parseInteger(token, &(newOperand->field)) && (newOperand->field == 1 || newOperand->field == 2)){
        newOperand->lable = (char*)malloc((strlen(structLabel)+1)* sizeof(char));
        if(newOperand->lable == NULL){
            printf("Error at line %d! Failed to allocate memory.\n", lineCount);
            free(newOperand);
            return NULL;
        }
        strcpy(newOperand->lable,structLabel);
        newOperand->addressingMode = STRUCT_ADDRESSING;
        return newOperand;
    }

/* if reached here, the operand doesnt fit any legal operand type*/
    printf("Error at line %d! Invalid characters.\n", lineCount);
    free(newOperand->lable);
    free(newOperand);
    return NULL;
}


/* Parses a directive.
 * First checks if the directive name is valid, then parses the directive arguments according to directive type.
 * If errors are found, prints an error and returns NULL.
 * Upon success returns a pointer to directive struct.*/
Directive* parseDirective(char *token, char *line){
    enum directiveType newDirectiveType;
    Directive* newDirective;

    token++; /*removing the leading dot character*/
    newDirectiveType = isValidDirective(token);

    /* check if directive name is valid */
    if(newDirectiveType == INVALID_DIRECTIVE){
        printf("Error at line %d! Undefined or missing directive.\n", lineCount);
        return NULL;
    }

    /* create new directive struct, initialise fields to NULL */
    newDirective = (Directive*)malloc(sizeof(Directive));
    if(newDirective == NULL){
        printf("Error at line %d! Failed to allocate memory.\n", lineCount);
        return NULL;
    }
    newDirective->directiveType = newDirectiveType;
    newDirective->dataParams = NULL;
    newDirective->labelParam = NULL;
    newDirective->stringParam = NULL;

    /* parse argumets according to directive type */

    if (newDirectiveType == DATA){
        newDirective->dataParams = parseData(line);
        if (newDirective->dataParams == NULL){
            directiveDestroy(newDirective);
            return NULL;
        }
           }

    else if (newDirectiveType == STRING) {
        newDirective->stringParam = parseString(line);
        if (newDirective->stringParam == NULL) {
            directiveDestroy(newDirective);
            return NULL;
        }
           }

    else if (newDirectiveType == STRUCT) {
        line = getNextCommaSeparatedToken(line, token);
       if (*line == ','){  /*skip the comma*/
            line++;
       }
            newDirective->dataParams = parseData(token);
            newDirective->stringParam = parseString(line);
            if (newDirective->stringParam == NULL || newDirective->dataParams == NULL) {
                directiveDestroy(newDirective);
                return NULL;
            }
          }

    else if (newDirectiveType == ENTRY || newDirectiveType == EXTERN){
        line = getNextToken(line,token);
        if (*token == '\0'){
            printf("Error at line %d! Missing label argument in directive.\n", lineCount);
            directiveDestroy(newDirective);
            return NULL;
        }

        newDirective->labelParam = parseLabel(token);
        if (newDirective->labelParam == NULL) {
            printf("Error at line %d! Failed to parse label argument in directive.\n", lineCount);
            directiveDestroy(newDirective);
            return NULL;
        }

        if(searchForExtraCharacters(line)){
            directiveDestroy(newDirective);
            return NULL;
            }
        }
    return newDirective;
}



/* Parses an instruction.
 * First checks if the dinstruction name is valid, then parses the instruction operands according to instruction type.
 * If errors are found, prints an error and returns NULL.
 * Upon success returns a pointer to instruction struct.*/
Instruction* parseInstruction(char *token, char *line){
    enum instructionType newInstructionType;
    Instruction* newInstruction;

    /* check if valid instruction */
    newInstructionType = isValidInstruction(token);
    if(newInstructionType == INVALID_INSTRUCTION){
        printf("Error at line %d! Undefined instruction.\n", lineCount);
        return NULL;
    }

   /* allocate instruction memory and initialize fields */
    newInstruction = (Instruction*)malloc(sizeof(Instruction));
    if(newInstruction == NULL){
        printf("Error at line %d! Failed to allocate memory.\n", lineCount);
        return NULL;
    }
    newInstruction->srcOperand = NULL;
    newInstruction->destOperand = NULL;
    newInstruction->instructionType = newInstructionType;

    /* parse operands */
    if(instructionsDescript_table[newInstructionType].srcOperand == 1){ /* if source operand is expected*/
        line = getNextCommaSeparatedToken(line, token);
        if(*line == ',') line++;      /*skip the comma*/
        if(strlen(token) == 0){
            printf("Error at line %d! Source operand is missing.\n", lineCount);
            instructionDestroy(newInstruction);
            return NULL;
        }
        newInstruction->srcOperand = parseOperand(token);
        if(newInstruction->srcOperand == NULL){
            instructionDestroy(newInstruction);
            return NULL;
        }
    }
    if(instructionsDescript_table[newInstructionType].destOperand == 1){ /* if destination operand is expected*/
        line = getNextCommaSeparatedToken(line, token);
        if(strlen(token) == 0){
            printf("Error at line %d! Destination operand is missing.\n", lineCount);
            instructionDestroy(newInstruction);
            return NULL;
        }
        newInstruction->destOperand = parseOperand(token);
        if(newInstruction->destOperand == NULL){
            instructionDestroy(newInstruction);
            return NULL;
        }
    }

    /* check for additional characters in this line*/
    if(searchForExtraCharacters(line)){
        instructionDestroy(newInstruction);
        return NULL;
    }

    /* check if the operands addressing mode is legal for this instruction type*/
    if(!validateAddressingMode(newInstruction)){
        instructionDestroy(newInstruction);
        return NULL;
    }

    return newInstruction;
}



/* Parses input line.
 * Returns NULL if there's an error while parsing the line.
 * Returns pointer to Line struct with NULL fields if the line is a comment or a blank*/
Line* parseLine(char *line){
    Line* lineStruct;
    char* newLabel;
    char token[LINE_BUFFER];


    line = getNextToken(line, token);

    /* allocate line struct memory and initialize fields */
    lineStruct = (Line*)malloc(sizeof(Line));
    if(lineStruct == NULL){
        printf("Error at line %d! Failed to allocate memory.\n", lineCount);
        return NULL;
    }
    lineStruct->label = NULL;
    lineStruct->directive = NULL;
    lineStruct->instruction=NULL;

    /*return empty line struct if empty row or comment*/
    if(*token == ';' || strlen(token) == 0) return lineStruct;

    /* parse label if exists */
    if (token[strlen(token)-1] == ':'){
        token[strlen(token)-1] = '\0';
        newLabel = parseLabel(token);
        if (newLabel == NULL) {
            free(lineStruct);
            return NULL;
        }
        lineStruct->label = newLabel;
        line = getNextToken(line, token);
    }

    if(strlen(token) == 0){
        printf("Error at line %d! Missing Instruction.\n", lineCount);
        lineDestroy(lineStruct);
        return NULL;

    }

    /* parse directive */
    if (*token == '.'){
        lineStruct->directive = parseDirective(token,line);

    /* for entry and extern directives set the label to NULL (ignoring the label)*/
        if((lineStruct->directive != NULL) && ((lineStruct->directive->directiveType == ENTRY) || (lineStruct->directive->directiveType == EXTERN)) && (lineStruct->label != NULL)){
            free(lineStruct->label);
            lineStruct->label = NULL;
        }
    }

    /* parse instruction */
    else lineStruct->instruction = parseInstruction(token, line);

    /* if instruction and directive parsing both failed, free line memory and return NULL  */
    if(lineStruct->instruction == NULL && lineStruct->directive == NULL){
        lineDestroy(lineStruct);
        return NULL;
    }

    return lineStruct;
}


