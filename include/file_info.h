#ifndef _FILE_INFO_H
#define _FILE_INFO_H

#include<fstream>
#include<vector>
#include<map>
#include"asm_defines.h"

struct file_info
{
  std::ifstream * input_file;
  std::vector<std::string> * input;
  std::map<std::string, word> * labels;
  dword * real_lines;
  dword num_lines;

  ~file_info();

  file_info(std::ifstream * input_file, std::vector<std::string> * input, std::map<std::string, word> * labels);

  void set_lines(dword num_lines);

};

#endif
