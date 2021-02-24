@ This is a sample Program
@ We will simply loop around and
@ Print the string "Hello World!"
MOV: R0, #40 @ Load the address into R0
LDRH: R1, R0 @ Load the byte char into register 1
PRNR: R1, char
ADD: R0, R0, #1 @ increment the address in R0
CMP: R1, #0
NEMOV: R14, #4
SUB: R15, R15, #4
LDR: R0, R15
PRNR: R0, int
END:
.data
"Hello World!"
9
