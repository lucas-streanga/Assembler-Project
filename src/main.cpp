#include<stdio.h>
#include<iostream>
#include<string>
#include<stdint.h>
#include<stdlib.h>
#include<vector>
#include"asm_defines.h"
#include"error_handler.h"
#include"init.h"
#include"Virtual_memory.h"
#include"decode.h"
#include<cstring>
#include <arpa/inet.h>

bool error_occurred;

#if SHOW_ALLOC == 1
#include <new>

struct alloc_details
{
  word total_bytes_allocated = 0;
  word total_bytes_free = 0;
  word number_of_allocs = 0;
  word number_of_frees = 0;

  word current_usage()
  {
    return total_bytes_allocated - total_bytes_free;
  }

};

static alloc_details s_alloc_details;

void* operator new(std::size_t size)
{
    s_alloc_details.total_bytes_allocated += size;
    s_alloc_details.number_of_allocs++;
    ALLOC_PRINT("New object created on heap with size " << size);
    void* result = malloc(size);
    if (result) return result;
    else
      throw std::bad_alloc();
}
void operator delete(void* ptr)
{
  s_alloc_details.number_of_frees++;
  ALLOC_PRINT("Object deleted off of heap.");
  if (ptr)
    free(ptr);
}
void* operator new[](std::size_t size)
{
  return operator new(size);  // defer to non-array version
}
void operator delete[](void* ptr)
{
  operator delete(ptr);  // defer to non-array version
}
#endif

int main(int argc, char ** argv)
{
  TIMER("Whole Program");
{
  error_occurred = 0;
  if(argc < 2)
    error_handler(ERR_NFS, 0, NULL);

  CHK_ERR;

  std::ofstream file(argv[1], std::ios::out | std::ios::binary);
  if(!file.is_open())
    error_handler(ERR_UOF, 0, argv[1]);

  CHK_ERR;
  std::vector<std::string> input;

  get_input(input);
  CHK_ERR;
  format(input);
  CHK_ERR;
  format_string_literals(input);

  LOG("\n***Formatted Program***\n");
  for(dword i = 0; i < input.size(); i++)
    LOG(input[i]);

  //Testing calculating number of words necessary...
  dword size = get_size(input);
  CHK_ERR;

  LOG("Memory Required:\n" << size << " words\n" << size*4 << " bytes");

  //Terminator for the input...

  Virtual_memory mem(size * 4);
  CHK_ERR;

  dword i = 0;
  while(input[i] != ".data")
  {
    word ins = get_instruction(input[i], i+1);
    memcpy(mem.data + (i * 4), &ins, 4);
    CHK_ERR;
    i++;
  }
  i++;
  while(input[i] != "!")
  {
    word data = get_data(input[i]);
    memcpy(mem.data + ((i - 1) * 4), &data, 4);
    i++;
  }
  print_all_memory(mem);
  file.seekp(0);
  file.write((const char *) mem.data, mem.size);
}
  ALLOC_PRINT("*** Total objects allocated " << s_alloc_details.number_of_allocs);
  ALLOC_PRINT("*** Total objects freed " << s_alloc_details.number_of_frees);

  return 0;
}
