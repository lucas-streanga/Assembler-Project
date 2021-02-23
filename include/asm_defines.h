#ifndef _ASM_DEFINES_H
#define _ASM_DEFINES_H

#include<stdint.h>

/* Terminal Colors */
#define TERM_COLOR_RESET "\033[0m"
#define TERM_COLOR_WARN "\033[1m\033[33m"
#define TERM_COLOR_LOG "\033[35m"
#define TERM_COLOR_ERROR "\033[1m\033[31m"
#define TERM_COLOR_TIMER "\033[32m"

#if DEBUG == 1
#include<iostream>
#define LOG(x) std::cout << TERM_COLOR_LOG << x << TERM_COLOR_RESET << std::endl
#include<iomanip>
#else
#define LOG(x)
#endif

#define MAX_U         4294967295L
#define MAX_S         2147483647L
#define SIGN_BIT      (-(2147483648L))

//If an error occurs...
extern bool error_occurred;
#define CHK_ERR if(error_occurred){return 0;}

/* Opcodes */
#define OP_ADD    0
#define OP_SUB    1
#define OP_LDR    2
#define OP_LDRH   3
#define OP_LDRB   4
#define OP_STR    5
#define OP_STRH   6
#define OP_STRB   7
#define OP_MOV    8
#define OP_CMP    9
#define OP_PRNR   10
#define OP_PRNM   11
#define TOTAL_INS 11

#define OP_END    255

/* Conditionals */
#define COND_AL 0
#define COND_EQ 1
#define COND_NE 2
#define COND_CS 3
#define COND_CC 4
#define COND_MI 5
#define COND_PL 6
#define COND_OS 7
#define COND_OC 8
#define COND_HI 9
#define COND_LS 10
#define COND_GE 11
#define COND_LT 12
#define COND_GT 13
#define COND_LE 14
#define COND_NV 15

/* Error Codes */

// String literal formatting
#define ERR_SLF   0
// literal is not a number
#define ERR_LNN   1
// Literal is too long
#define ERR_LTL   2
// Unable to allocate memory
#define ERR_UTA   3
// Format of opcode
#define ERR_FOP   4
//Unknown opcode
#define ERR_UOC   5
//Unknown conditional
#define ERR_UCO   6
//Incorrect Number of args
#define ERR_INA   7
//Immediate value prefix not present
#define ERR_IVP   8
//Unable to resolve register
#define ERR_URR   9
//Unable to resolve immediate value
#define ERR_URI   10
//No file specified
#define ERR_NFS   11
//Unable to open file
#define ERR_UOF   12


#define AS_IMMED_MAX    (0b00000000000000000000001111111111)
#define AS_SHIFT_MAX    (0b00000000000000000000000000111111)
#define LS_IMMED_MAX    (0b00000000000000000011111111111111)
#define LS_SHIFT_MAX    (0b00000000000000000000001111111111)
#define PRNM_IMMED_MAX  (0b00000000000000000011111111111111)
#define PRNM_SHIFT_MAX  (0b00000000000000000000001111111111)

/* Typedefs */
typedef uint8_t byte;
typedef uint16_t half;
typedef uint32_t word;
typedef uint64_t dword;
/* END */



#endif
