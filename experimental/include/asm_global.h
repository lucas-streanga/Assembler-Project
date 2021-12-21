#ifndef _ASM_DEFINES_H
#define _ASM_DEFINES_H

#include<inttypes.h>

/* Terminal Colors */
#define TERM_COLOR_RESET "\033[0m"
#define TERM_COLOR_WARN "\033[1m\033[33m"
#define TERM_COLOR_LOG "\033[35m"
#define TERM_COLOR_ERROR "\033[1m\033[31m"
#define TERM_COLOR_TIMER "\033[32m"
#define TERM_COLOR_ALLOC "\u001b[34m"

//Error macros
#define FATALERROR(x) std::cerr << TERM_COLOR_ERROR << "\n" << x << "\n\nExitting now..." << TERM_COLOR_RESET << '\n'
#define NONFATALERROR(x) std::cerr << TERM_COLOR_WARN << x << TERM_COLOR_RESET << '\n'

//default out file
#define DEFAULT_OUT_FILE "a.out"

/* Debugging for LOG macro and also new overloading to view mallocs on heap. */
#if DEBUG == 1
#include<iostream>
#define LOG(x) std::cout << TERM_COLOR_LOG << x << TERM_COLOR_RESET << '\n'
#include<iomanip>
#else
#define LOG(x)
#endif

#if SHOW_ALLOC == 1
#include<iostream>
#define ALLOC_PRINT(x) std::cout << TERM_COLOR_ALLOC << x << TERM_COLOR_RESET << '\n'
#include<iomanip>
#else
#define ALLOC_PRINT(x)
#endif

#define MAX_U         4294967295L
#define MAX_S         2147483647L
#define SIGN_BIT      (-(2147483648L))

#define AS_IMMED_MAX    (0b00000000000000000000001111111111)
#define AS_SHIFT_MAX    (0b00000000000000000000000000111111)
#define LS_IMMED_MAX    (0b00000000000000000011111111111111)
#define LS_SHIFT_MAX    (0b00000000000000000000001111111111)
#define PRNM_IMMED_MAX  (0b00000000000000000011111111111111)
#define PRNM_SHIFT_MAX  (0b00000000000000000000001111111111)
#define BRN_SHIFT_MAX   (0b00000000000001111111111111111111)

//For printing correct size ints
#define __STDC_FORMAT_MACROS

/* Typedefs */
typedef uint8_t byte;
typedef uint16_t half;
typedef uint32_t word;
typedef uint64_t dword;

//Now we will define a timer
#if TIME == 1
#define TIMER(x) timer t(x); //Will create a timer when time is called...
//We will put our timer struct definition in here as well...
#include<chrono>
#include<string>
#include<iostream>
struct timer
{
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  std::string name;

  timer(const char * n)
  {
    name = n;
    start = std::chrono::high_resolution_clock::now();
  }

  ~timer()
  {
    end = std::chrono::high_resolution_clock::now();
    std::cout << TERM_COLOR_TIMER << "***TIMER: " << name << " took " << ((std::chrono::duration<float>)(end - start)).count() * 1000 << " ms." << TERM_COLOR_RESET << '\n';
  }
};
#else
#define TIMER(x)
#endif

#define NUM_CONDITIONALS 16

//Conditional lookup table
const char COND_LT[NUM_CONDITIONALS][3] =
{
"al",
"eq",
"ne",
"cs",
"cc",
"mi",
"pl",
"os",
"oc",
"hi",
"ls",
"ge",
"lt",
"gt",
"le",
"nv"
};

#define OP_END 255
#define NUM_OPCODES 14

//opcode lookup table
const char OPCODE_LT[NUM_OPCODES][5] =
{
  "add",
  "sub",
  "ldr",
  "ldrh",
  "ldrb",
  "str",
  "strh",
  "strb",
  "mov",
  "cmp",
  "prnr",
  "prnm",
  "brnl",
  "brn"
};



#endif
