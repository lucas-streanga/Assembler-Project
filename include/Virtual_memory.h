#ifndef _H_VIRTUAL_MEMORY
#define _H_VIRTUAL_MEMORY

#include"asm_defines.h"
#include<cstdlib>

struct Virtual_memory
{
  byte * data;
  size_t size;

  Virtual_memory(size_t size);

  byte& operator[](dword index);

  ~Virtual_memory();

};

#endif
