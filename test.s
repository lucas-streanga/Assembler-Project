ADD: R0, R0, R1 @ Add R0 and r0 and store in R1

@ This file is to test parsing

eqADDS: R15, R0, R0, #15
SUB:    R15, R15, #10
LDRH:   R0, #8
MOV:    R0, R15, #8
CMP:    R0, #10000
PRNR:   R15, R10, R0, R12, int
EQPRNM:   #100, byte, char
END:
.data

4294967295
"Hello World!"
