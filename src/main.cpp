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
#include<map>

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

  std::ofstream out_file;
  std::ifstream in_file;

  std::vector<std::string>  input;
  std::map<std::string, word>  labels;

  file_info info(&in_file, &input, &labels);

  if(argc < 2)
    error_handler(ERR_NFS, 0, &info, NULL);
  CHK_ERR;
  if(argc < 3)
  {
    printf("No output file specified, default %s will be used.\n", DEFAULT_OUT_FILE);
    out_file.open(DEFAULT_OUT_FILE, std::ios::out | std::ios::binary);
  }
  else
  {
    out_file.open(argv[2], std::ios::out | std::ios::binary);
  }

  in_file.open(argv[1]);


  if(!out_file.is_open() || !in_file.is_open())
    error_handler(ERR_UOF, 0, &info, argv[1]);

  CHK_ERR;

  //We will use vector reserve semantics to reduce the number of reallocs
  dword num_lines = reserve_line_count(in_file);
  info.set_lines(num_lines);
  input.reserve(num_lines);

  get_input(info);
  CHK_ERR;
  format(info);
  CHK_ERR;
  resolve_labels(input, labels);
  CHK_ERR;

  //Debug log the labels.
  LOG("\nLabel and Address Resolution:");
  for(std::map<std::string, word>::iterator it = labels.begin(); it != labels.end(); it++)
  {
    LOG(it->first << " " << it->second);
  }

  format_string_literals(input);
  CHK_ERR;

  LOG("\n***Formatted Program***\n");
  for(dword i = 0; i < input.size(); i++)
    LOG(input[i]);
  LOG("\n");

  //Testing calculating number of words necessary...
  dword size = get_size(input);
  CHK_ERR;

  //Terminator for the input...

  Virtual_memory mem(size * 4);
  CHK_ERR;

  dword i = 0;
  while(input[i] != ".data" && input[i] != "!")
  {
    word ins = get_instruction(info, input[i], i+1, labels);
    CHK_ERR;
    memcpy(mem.data + (i * 4), &ins, 4);
    i++;
  }
  if(input[i] != "!")
    i++;
  while(input[i] != "!")
  {
    word data = get_data(input[i]);
    memcpy(mem.data + ((i - 1) * 4), &data, 4);
    i++;
  }
  print_all_memory(mem);
  out_file.seekp(0);
  out_file.write((const char *) mem.data, mem.size);
}
  ALLOC_PRINT("*** Total objects allocated " << s_alloc_details.number_of_allocs);
  ALLOC_PRINT("*** Total objects freed " << s_alloc_details.number_of_frees);
  ALLOC_PRINT("*** Total bytes allocated " << s_alloc_details.total_bytes_allocated);

  return 0;
}
