/*****************************
 * Olga Danielle Cahana
 * 317171452
 * **************************/

#include <stdio.h>
#include "compile_file.h"

/* This is the main function.
 * It compiles all the files provided as program arguments, one by one.
 * It notifies the user if errors occurred at any stage of file processing.*/
int main(int argc, char** argv){
if( argc == 1){
    printf("Need to provide at least one input file.\n");
    return -1;
}
    while (--argc){
        switch (compileFile(*++argv)){
            case COMPILE_SUCCESS:
                break;
            case COMPILE_FAILURE_READ_FILE:
                printf("Failed open %s file.\n", *argv);
                break;
            case COMPILE_FAILURE_MEMORY_ALLOCATION:
                printf("Failed to allocate memory at %s input compilation.\n", *argv);
                break;
            case COMPILE_FAILURE_FIRST_PASS:
                printf("Detected errors at first pass in %s input. Program will not proceed to second pass.\n", *argv);
                break;
            case COMPILE_FAILURE_SECOND_PASS:
                printf("Detected errors at second pass in %s input. Program will not proceed to output generation.\n", *argv);
                break;
            case COMPILE_FAILURE_WRITE_TO_FILE:
                printf("Failed to generate output files for %s input.\n", *argv);
                break;
        }
    }
        return 0;
}




