; This is a test input file written by the GRADER.
; Write your own assembly language source program.
; The program should contain at least 10 insructions, 5 labels, 5 directives
; No errors: the assembler should generate successfully all output files:
;     myprog.ob, myprog.ext, myprog.ent

STR:		.string 	"abcdef"
LENGTH:		.data		6,-9 , 15, 0
K:			.data		22
S1:			.struct		8, "ab"
S2:			.struct		5, "mytest"

			.entry		LOOP
	 		.entry		LENGTH
			.extern		L3
			.extern		W

MAIN:		mov			S1.1,W
			mov 		S1.2 ,r3
			cmp			#56, W
			add			#23, r4
			add			r2,STR
			sub			r2, S1.1
			sub			r1, r4
			not			K
			clr			r2
			lea			LENGTH, L3
			inc			S2.2
			dec			K
			jmp			END
			bne			LENGTH
			red			r1
			prn			#4
			jsr			K
			rts			
			add			r2,STR
LOOP:		jmp			W
			prn 		#-5
			inc 		K

			bne 		L3
END:		stop

