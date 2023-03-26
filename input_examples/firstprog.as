
; file ps.as
;;;;;;;;;;;;;;;;;This is my first prog!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; Tester


; Illegal label(save word)
     	
r1:		mov #1, a
stop: 	

; Illegal adress types for operations
	
	mov #1, #8
	cmp L(#1,#2)
	not #999
	lea r1, a
	lea a, #9
	jmp #8
	jsr #9
	
;This is legal
	jmp K1(r1,r3)
	

; entry and extern errors(amount of labels) and warnings


ABD:    .entry LENGTH , 5
 .extern
ac: 				.extern W , aaff
5AIN: 	mov r3 ,LENGTH
; Legal lines
LOOP: 	jmp L1(#-1,r6)
	mcr myMacro
	      sub r1, 		r4
	      bne L3
	endmcr
	myMacro
L1:	inc K
.entry LOOP

; Illegal label

	bne LOOP(K,W****)
END:	stop

; Illegal string

STR:    .string "abcdef
;Two labels with the same name
STR:    .string "aasfafs" 
        .string "aasfafs"     asfasfasfsafafasfasf!!

; Illegal instruction
LENGTH .data -6 -9,15   

; data guidance errors and instruction errors

LENGTH: .data -6 -9,15


K:      .data +22 ,, 5
	
		
		
        .data 22 ,, 5.5
        .string
        .data
        .data +-9 
        .data 9+ 
        .data 9-
        
; Illegal instruction
        
     				hello
     				LENGTH:			,
     	
; Illegal numbers
     	
     	mov ##1, abc
     	mov # , #5
     	mov 1, 2
     	mov #+ , abc
     	mov #9+89898 , GG
     	
; Illegal label
     	
     	bne !temp
     	jmp 1LR
     	
; Illegal register
     	
     	bne r10
     	
; Errors in third adress type(of jmp, bne and jsr operations)
     	
     	jmp L1(#5,   N)
     	jmp L1(#5 ,N)
     	jmp 1(#5 , N)
     	jmp 1(#5,N)
     	jmp L( #5,N)
     	jmp L(#5,9
     	jmp L #5,9
     	
; Illegal operations
     	
     	mo
     	st
     	cm1
     	ret
     	ads
     	
; Illegal amount of operands in operations
     	
     	mov 
     	cmp abc
     	add r1, r2 , r3 		, r5
     	lea ,
     	not
     	inc a, b ,c , #5
     	rts r1
     	stop 				#1 	, r1
     	stop K
     	
; Errors in commas in instruction line
     	
     	mov #1 , 
     	cmp r1 ,, r2
     	jmp ,   r
     	stop ,
     	mov #1 , #2 				,
     	
; Errors in registers
     	
     	mov r9, r1
	
;This is legal
	jmp K1(r1, r3)
	
     	
     	
     	
     	
	
	
		
			
				
				
				
