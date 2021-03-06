; This file contains errors in .struct directives and struct operands.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; All errors should be detectable in the first pass of the assembler.

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; Disclaimer: this list of errors is not exhaustive; 
;             you are encouraged to identify additional errors.

; 1. this line is ok
STRCT1: .struct 100,"abcxyz123!@#"

; 2. this line is ok 
STR1:   .string "abcdefg"

; 3. this line is ok (not mandatory to provide a label)
        .struct -33,"123456"

; 4. invalid characters (xyz)
        .struct 4,xyz

; 5. invalid characters (STRCT1,"qwerty")
        .struct STRCT1,"qwerty"
				
; 6. invalid characters (r3,"qwerty")
        .struct r3,"qwerty"
		
; 7. invalid characters (r4)
        .struct 3,r4

; 8. invalid characters ("1234",2)
        .struct "1234",2

; 9. missing arguments in directive 
        .struct 
		
; 10. missing argiment in directive 
        .struct 4

; 11. missing agrument in directive 
        .struct 4,

; 12. invalid characters (,"12345")
        .struct -4,"8765","12345"

; 13. invalid characters (67)
        .struct -4,"12345" 67
				
; 14. invalid characters (3)
        cmp r1,STRCT1.3

; 15. this line is ok (struct label may be used as direct operand)
        cmp STRCT1,r1

; 16. this line is ok (any label may be used for struct operand)
        cmp r1,STR1.1

; 17. invalid characters (.2)
        prn STRCT1.1.2

; 18. invalid characters(r2)
        prn STRCT1.r2

; 19. invalid characters (blabla)
        cmp #3,STRCT1.1 blabla
