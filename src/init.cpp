#include"init.h"
#include"asm_defines.h"
#include"Virtual_memory.h"
#include<cstring>

void inline STRING_TO_LOWER(std::string& s)
{
  for(dword i = 0; i < s.size(); i++)
  {
    if(s[i] == '"')
    {
      i++;
      while(s[i] != '"' && i < s.size())
        i++;
    }
    s[i] = std::tolower(s[i]);
  }
}

//And one for removing whitespace in a string.
void inline STRING_RM_WHITESPACE(std::string& s)
{
  std::string temp;
  for(dword i = 0; i < s.size(); i++)
  {
    if(s[i] == '"')
    {
      temp.push_back(s[i]);
      i++;
      while(s[i] != '"' && i < s.size())
      {
        temp.push_back(s[i]);
        i++;
      }
    }
    if(!isspace(s[i]))
      temp.push_back(s[i]);
  }

  s = temp;
}

void format_string_literals(std::vector<std::string> & input)
{
  for(dword i = 0; i < input.size(); i++)
  {
    //String lit encountered
    //We will put 4 chars per line now
    if(input[i][0] == '"')
    {
      for(dword j = 0; j < input[i].size(); j++)
        if(input[i][j] == '"')
        {
          input[i].erase(j, 1);
          j--;
        }

      input[i].push_back('\0');
      for(dword z = 0; z < input[i].size(); z+=4)
      {
        char fourchar[6] = {0};
        fourchar[0] = ',';
        strcpy(fourchar + 1, input[i].substr(z, 4).c_str());
        input.insert(input.begin() + i + (z/4) + 1, fourchar);
      }
      input.erase(input.begin() + i);
    }
  }
  input.push_back("!");
}

void get_input(std::vector<std::string> & input)
{
  while(std::cin)
  {
    input.push_back("");
    getline(std::cin, input[input.size() - 1]);
  }
}

void format(std::vector<std::string> & input)
{
  bool reached_data = false;
  for(dword i = 0; i < input.size(); i++)
  {
      std::string & input_line = input[i];
      for(dword j = 0; j < input_line.size(); j++)
        if(input_line[j] == '@')
          input_line.erase(input_line.begin() + j, input_line.end());

      STRING_RM_WHITESPACE(input_line);
      STRING_TO_LOWER(input_line);

      if(input_line == ".data")
      {
        reached_data = true;
        continue;
      }

      if(reached_data)
        if(input_line.find('"') == std::string::npos && !input_line.empty())
        {
          try
          {
            for(dword k = 0; k < input_line.size(); k++)
              if(!std::isdigit(input_line[k]))
              {
                error_handler(ERR_LNN, i+1, input_line.c_str());
                return;
              }

            dword w = std::stoll(input_line.c_str());
            if(w > MAX_U)
            {
              error_handler(ERR_LTL, i+1, input_line.c_str());
              return;
            }
          }
          catch(const std::out_of_range& e)
          {
            error_handler(ERR_LTL, i+1, input_line.c_str());
            return;
          }
        }

      if(input_line.empty())
      {
        input.erase(input.begin() + i);
        i--;
      }
  }
  return;
}

dword get_size(std::vector<std::string> & input)
{
  dword num_words = 0;
  for(dword i = 0; i < input.size(); i++)
  {
    if(input[i] != ".data" && input[i] != "!")
      num_words++;
  }

  return num_words;
}

void print_all_memory(Virtual_memory &mem)
{
  printf("***PRINTING MEMORY***\n");
  for(size_t i = 0; i < mem.size; i += 4)
    printf("%-12lu%010lu\t%lX\n", i, *((word *)(mem.data + i)), *((word *)(mem.data + i)));
  printf("***DONE***\n");
}
