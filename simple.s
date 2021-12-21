@ This is a simple hello world program!
MOV: R0, #0  @ Load the address into R0
.start_loop
LDRH: R1, R0, .msg  @ Load the byte char into register 1
PRNR: R1, char
ADD: R0, R0, #1 @ increment the address in R0
CMP: R1, #0
NEBRN: .start_loop
END:
.data
.msg
"Hello World!"
