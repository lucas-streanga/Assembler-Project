# Assembler-Project
An Assembler that converts assembly language for the virtual processor into machine code for the processor.
Please also see [Virtual Processor](https://github.com/lucas-streanga/Processor-Project)

# How it works
This assembler allows the user to write assembly language and will assemble it into binary machine code for use with the virtual processor. 
Currently, all instructions are supported, as well as conditionals, flags, and data in memory. Labels are not yet implemented, but branching
can be done by editing the PC.

  # Compilation
  
  The program includes a Makefile for compilation. Running **make** will compile the program as expected. **make debug** can be used to enable debug printouts during program execution to check instruction decoding and other details. **make cleanup** is used to delete object files. No dependencies other than GCC, make, and the C and C++ standard libraries are required. 

# Sample Program
```
@ This is a sample Program
@ We will simply loop around and
@ Print the string "Hello World!"
MOV: R0, #28 @ Load the address into R0
LDRH: R1, R0 @ Load the byte char into register 1
PRNR: R1, char
ADD: R0, R0, #1 @ increment the address in R0
CMP: R1, #0
NEMOVS: R14, #4
END:
.data
"Hello World!"
```
This program will load and print one character at a time from the string in memory until the null terminator is encountered. 
```
Default memory size of 16384 bytes in use
Initializing Memory...
Memory Initialized Successfully.
CPU Initialized Successfully.
Loading program into memory...

***EXECUTION***

R1: H
R1: e
R1: l
R1: l
R1: o
R1:  
R1: W
R1: o
R1: r
R1: l
R1: d
R1: !
R1: 

End opcode recieved.
Execution ended.
Virtual Cycles used: 359
```

## Usage
The first and only commandline parameter is the file to save. The assembly language file is redirected in with **<**.

## Assembly File Formatting 
The user program should be written in plain text. Comments can be added with the **@** character
and will be ignored. Whitespace is ignored as well. Capitalization is ignored in everything except string literals. 
All operations must seperate the operands and the opcode
with a **:**. The first two characters of the operation may be used for a conditional, and the last character 
may be used for S, if the flags should be set. Literals in operations must be prefaced with a **#**. The label
*.data* must be used to indicate data that is not instructions. In the data sections, literals do not require
**#**, but string literals must be contained in quotations (**"**) and a null-terminator (character 0) will always be
appended to them.

## Assembly Errors

The Assembler may encounter issues with format in the file. Any improper format will end execution, and will not
write out to the file. If the file is unwritable, an error occurs as well. Each error code will attempt
to print the line in the file that the error occurs. The error codes will also print the potential problem
as well as a couple solutions. 

### Program memory allocation details
Virtual memory is variable from run to run and is allocated on the heap. Many strings are
also heap-allocated, to avoid potentially long input crashing the program. Everything else
is auto stack allocated. 

# Opcodes
The current defined opcodes are as follows:
- 0: ADD
  - Addition
- 1: SUB
  - Subtraction
- 2: LDR
  - Load one word from memory.
- 3: LDRH
  - Load one half-word from memory.
- 4: LDRB
  - Load one byte from memory.
- 5: STR
  - Store one word to memory.
- 6: STRH
  - Store one half-word to memory.
- 7: STRB
  - Store one byte to memory.
- 8: MOV
  - Move value into register.
- 9: CMP
  - Compare the values of two registers.
- 10: PRNR
  - Print content of up to 4 registers.
- 11: PRNM
  - Print value at memory address.
- 255: END
   - End execution.
## Registers
The processor has 16 register available to the user, R0 - R15. R14 is the program counter and can be edited for loops, and R15 is the stack pointer
which can be used to read data from memory. The processor also has another register not accessible, the IR or instruction register. This hold the 
current instruction in execution. 

## Execution Flags
The processor includes 4 flags, Z (zero), N (negative), O (overflow), and C (carryover). The flags are only set after execution of instruction if the S bit is set to 1 in the instruction. 
The Zero flag is set if the result is 0, the Negative flag is set if the result is negative (MSB is 1), the Overflow flag is set if an overflow occurs (sign bit corrupted) and the Carryover flag if the result exceeds 32 bits in length and is truncated. 

## Conditonals
The conditionals that determine if an instruction is executed or skipped is defined by 4 bits in the instruction. Therefore, there are 0 - 15 possible 
conditionals.

- 0000  AL
  - Always, regardless of flags set.
- 0001  EQ
  - Equal, if the Z flag is set.
- 0010  NE
  - Not equal, if Z flag is not set.
- 0011  CS
  - Carry set, if C flag is set.
- 0100  CC
  - Carry clear, if the C flag is not set. 
- 0101  MI
  - Negative, if the N flag is set.
- 0110  PL
  - Positive, if the N flag is not set.
- 0111  OS
  - Overflow set, if the O flag is set.
- 1000  OC
  - Overflow clear, if the O flag is not set.
- 1001  HI
  - Unsigned higher, if C is set and Z is clear.
- 1010  LS
  - Unsigned lower, if C is clear and Z is set.
- 1011  GE
  - Greater than or equals, if N and O are set or N and O clear.
- 1100  LT
  - Less than, if N set and O clear or N clear and O set.
- 1101  GT
  - Greater than, if Z is clear and either N and O set or N and O clear.
- 1110  LE
  - Less than or equals, if Z is set or N set and O clear or N clear and O set.
- 1111  NV
  - Never, never executes regardless of flags.
  
## Program Structure

The program is split up into four modules: main, initialization, virtual memory, and decoding. Main contains the main execution and opens the files and writes to the file. Init is used for formatting the file before it can be decoded, for easier decoding. This includes removing whitespace, capitalization, formatting string, etc. Decoding is the largest chunk of the program. Here, each line is decoded into one machine code instruction and the .data section is also converted to binary
data. The virtual memory is identical to virtual processor, and is used to store the program before it is written to a binary file. 
  
  
### Testing

The project was tested extensively with edge cases and improper input to test its functionality/robustness. Valgrind was used for the testing. Segmentation faults and memory leaks were impossible to create in my testing, and there are no improper read/write errors as well, even with faulty input instructions or even incomplete instructions. The only potential issue is on systems that over-commit on memory allocation. If a system over commits memory to malloc(), there is potential to crash the program as malloc() will not return a null pointer but will rather fail silently. This was not an issue on my system as the OS only allows allocation of memory that actually exists, but some systems allow malloc() of extremely large sizes, such as 1000gb, without returning a null pointer.
