;label errors
myLabel: .string "This should be fine"
1myLabel: .string "This shouldn't"
thisIsAveryVeryVeryVeryVeryLongLabel: .data 12, 4, 56, 78, 10
this label contains spaces in it: mov L1, L1
thisLabelIsErroneous  : sub r1, r4
myLabel: .string "This label was already used on first line"

I wil lealcasknc ake' jei erlaks ;ase ien asl ;sei ekcvma;slk oe kcamsl;ke iekcmasicoeh kea' slkcopaecjnas m;saji[]

;comma errors
myArray: .data 12,6, -9, 10,
    myArray2: .data 12,6,-9,10,,
    myArray3: .data ,12,6, -9, , 10
myArray4: .data 12,     ,    6,-9,10
myArray5: .data 12, 6, -9, 0, -10       ,
    mov, r3, K
cmp K, , #-6
sub ,r1, r4
add , L3  L3
add , L3,  L3
inc , r1
inc, r1


;line length errors
K1: .data 1200, 1234, 54,90,-23         ,       42224,          3466,   +554,  -7,  12,75553, 763, 345
K2: .string "I am a very long string that will surely exceed the maximum length of a line"

;data errors
    .data
    .data lost, 4, 8, 15, 16, 23, 42
    .data --433, 653, 30
    .data 763, 599, ++875, 5
    .data 4, 8, 1  5 , 16, 23, 42
    .data 4,8,15,16,23,4 +2
    .data  4, 8, 15, x, 16, 23, 42
    .data a, b, c, d
    .data 3, 4, 6.5, 9
    .data 2048
    .data -20490


;string errors
    .string
    .string za wardo
    .string za wardo"
    .string "za wardo
    .string za "wardo".
    .string za ."wardo"
    .string ",
;extraneous words
    .data 943 .data
    .string "sdf" .string
    .extern Hello World
    .entry Goodbye Earth
mov A, r1, r2
mov A, r1 r2
cmp A, r1, r2
cmp A, r1 r2
add A, r0, r1
add A, r0 r1
sub #3, r1, r5
sub #3, r1 r5
lea HAHAHA, r3, r4
lea HAHAHA, r3 r4
clr r2, r6
clr r2 r6
inc r1, r4
inc r1 r4
dec Count, Monte, Cristo
dec Count Monte Cristo
jmp %ROOF, %FLOOR
jmp %ROOF %FLOOR
bne Bob, Sponge
bne Bob Sponge
jsr BAT, MAN
jsr BAT MAN
red r3, r5
red r3 r5
prn r1, r2
prn r1 r2
customLabel1:.data 943 .data
customLabel3:.extern Hello World
customLabel26:jsr BAT MAN
customLabel27:red r3, r5
customLabel30:prn r1 r2
    .data
    .string
    .extern
    .entry
mov A
cmp A
add A
sub #3
lea HAHAHA
clr
inc
dec
jmp
bne
jsr
red
prn
someLabel31:.data
someLabel32:.string
someLabel33:.extern
someLabel34:.entry
someLabel35:mov A
someLabel36:cmp A
someLabel37:add A
someLabel38:sub #3
someLabel39:lea HAHAHA
someLabel40:clr
someLabel41:inc
someLabel42:dec
someLabel43:jmp
someLabel44:bne
someLabel45:jsr
someLabel46:red
someLabel47:prn
;unknown words
COOK tasty, pie
    .Data 12, 675
    .sTring "Whoops"
;operand addressing errors
mov %ohno, #1
mov %ohno, %ohyes
mov #3, %ohno
mov %ohno, r3
mov #-1, #5
mov #-1, %ohyes
cmp %ohno, %ohyes
add #3, #1
add %ohno, %ohyes
add %ohno, someLabel47
add %ohno, r3
add #-1, #5
add #-1, %ohyes
sub %ohno, #1
sub %ohno, %ohyes
sub %ohno, someLabel47
sub %ohno, r3
sub #-1, #5
sub #-1, %ohyes
lea #-1, #8
lea #-1, someLabel47
lea #-1, %ohno
lea #-1, r4
lea %ohno, #-1
lea %ohno, %ohyes
lea %ohno, someLabel47
lea %ohno, r0
lea r1, #-1
lea r1, %ohno
lea r1, someLabel47
lea r1, r0
lea someLabel47, #-1
lea someLabel47, %ohno
clr #-1
clr %ohno
not #-1
not %ohno
inc #-1
inc %ohno
dec #-1
dec %ohno
jmp #-1
jmp r1,r2
bne #-1
jsr #-1
prn %ohno
rts #-1
rts someLabel47
rts %ohno
rts r1
stop #-1
stop someLabel47
stop %ohno
stop r1


;	THINGS THAT WERE MISSING IN GIT:

;jump command errors:
jmp (r1,r2)
jmp L1(r1,r2
jmp L1r1,r2)
jmp 1L(r1,r2)
jmp L1(r1,r2)4
jmp L1()
jmp L1(r1)
jmp l1(r1,r2,r3)
jmp r1,r2
jmp L1r1,r2
jmp r1,r2)
clr r1,r2



