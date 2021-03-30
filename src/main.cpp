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
#include"alloc_details.h"
#include"buffer.h"
#include<cstring>
#include<map>

bool error_occurred;

int main(int argc, char ** argv)
{
  TIMER("Whole Program");
{
  error_occurred = 0;

  buffer<char> out_file_buffer(OUT_FILE_BUFFER_SIZE);

  std::ofstream out_file;
  std::ifstream in_file;

  //Using our own buffer with its own lifetime prevents early deletion and reallocation since we close and
  //reopen the file.
  out_file.rdbuf()->pubsetbuf(out_file_buffer.get_buffer(), out_file_buffer.get_size());

  std::vector<std::string>  input;
  std::map<std::string, word>  labels;

  file_info info(&in_file, &input, &labels);
  std::string out_file_name;

  if(argc < 2)
    error_handler(ERR_NFS, 0, &info, NULL);
  CHK_ERR;
  if(argc < 3)
  {
    printf("No output file specified, default %s will be used.\n", DEFAULT_OUT_FILE);
    out_file_name = DEFAULT_OUT_FILE;
  }
  else
  {
    out_file_name = argv[2];
  }

  //Open in append mode for now...
  out_file.open(out_file_name, std::ios::app | std::ios::binary);
  ALLOC_PRINT("This is a file handle: " << out_file_name);
  in_file.open(argv[1]);
  ALLOC_PRINT("This is a file handle: " << argv[1]);


  if(!out_file.is_open() || !in_file.is_open())
    error_handler(ERR_UOF, 0, &info, argv[1]);

  CHK_ERR;
  //We can close this for now.
  out_file.close();

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

  out_file.open(out_file_name, std::ios::out | std::ios::binary);
  if(!out_file.is_open())
    error_handler(ERR_UOF, 0, &info, out_file_name.c_str());
  CHK_ERR;

  out_file.seekp(0);
  out_file.write((const char *) mem.data, mem.size);
  //We can close the files now
  out_file.close();
  in_file.close();
  print_all_memory(mem);
}

  print_alloc_details();

  return 0;
}
