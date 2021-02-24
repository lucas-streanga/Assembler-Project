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

int main(int argc, char ** argv)
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


  return 0;
}
