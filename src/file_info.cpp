#include"file_info.h"

file_info::~file_info()
{
  if(real_lines != NULL)
    free(real_lines);
}

file_info::file_info(std::ifstream * input_file, std::vector<std::string> * input, std::map<std::string, word> * labels)
{
  this->input_file = input_file;
  this->input = input;
  this->labels = labels;
  real_lines = NULL;
}

void file_info::set_lines(dword num_lines)
{
  real_lines = (dword *) malloc(sizeof(dword) * num_lines);
  this->num_lines = num_lines;
  for(dword i = 0; i < num_lines; i++)
    real_lines[i] = 0;
  LOG("Number of lines in original file: " << (unsigned int) num_lines);
}
