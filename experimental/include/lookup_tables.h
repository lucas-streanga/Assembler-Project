#ifndef _H_ASM_LOOKUP
#define _H_ASM_LOOKUP

#include"asm_global.h"
#include<map>

class Lookup
{

  static Lookup * m_lookup;

  Lookup() {}

public:

  class label_table
  {
    std::map<std::string, dword> label_map;

  public:
    int size();
  };

  label_table labels;

  static Lookup & get();
  static void destroy();

  const char cond[NUM_CONDITIONALS][3] =
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

  //opcode lookup table
  const char opcode[NUM_OPCODES][5] =
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

};



#endif
