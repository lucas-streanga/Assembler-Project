#include"init.h"
#include"asm_defines.h"
#include"Virtual_memory.h"
#include<cstring>
#include<map>

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

dword reserve_line_count(std::ifstream & file)
{
  file.unsetf(std::ios_base::skipws);
  dword line_count = std::count(std::istream_iterator<char>(file), std::istream_iterator<char>(), '\n');
  LOG(line_count);
  file.clear();
  file.seekg(0);
  return line_count;
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

void get_input(file_info & info)
{
  LOG("***GETTING INPUT***");
  std::vector<std::string> & input = *(info.input);
  std::ifstream & file = *(info.input_file);

  std::string input_line;
  dword real_line = 0;
  dword i = 0;
  while(getline(file, input_line))
  {
    info.real_lines[i] = real_line + 1;

    for(dword j = 0; j < input_line.size(); j++)
      if(input_line[j] == '@')
        input_line.erase(input_line.begin() + j, input_line.end());

    STRING_RM_WHITESPACE(input_line);
    STRING_TO_LOWER(input_line);

    if(!input_line.empty())
    {
      i++;
      real_line++;
      input.push_back(input_line);
    }
    else
    {
      real_line++;
    }
  }

  LOG("***GETTING INPUT DONE***");
}

void format(file_info & info)
{
  bool reached_data = false;
  std::vector<std::string> & input = *(info.input);

  for(dword i = 0; i < input.size(); i++)
  {
      std::string & input_line = input[i];

      if(input_line == ".data")
      {
        reached_data = true;
        continue;
      }
      //This is a label. Skip it
      if(input_line[0] == '.')
        continue;

      if(reached_data)
        if(input_line.find('"') == std::string::npos && !input_line.empty())
        {
          try
          {
            for(dword k = 0; k < input_line.size(); k++)
              if(!std::isdigit(input_line[k]))
              {
                error_handler(ERR_LNN, i, &info, NULL);
                return;
              }

            dword w = std::stoll(input_line.c_str());
            if(w > MAX_U)
            {
              error_handler(ERR_LTL, i, &info, NULL);
              return;
            }
          }
          catch(const std::out_of_range& e)
          {
            error_handler(ERR_LTL, i, &info, NULL);
            return;
          }
        }

  }
  LOG("\nMapping of virtual lines to real lines in file:");
  for(dword i = 0; i < info.num_lines; i++)
    LOG((unsigned int) i << " " << (unsigned int) info.real_lines[i]);

  return;
}

dword get_size(std::vector<std::string> & input)
{
  dword num_words = 0;
  for(dword i = 0; i < input.size(); i++)
  {
    if(input[i][0] != '.' && input[i] != "!")
      num_words++;
  }

  return num_words;
}

void resolve_labels(std::vector<std::string> & input, std::map<std::string, word> & labels)
{
  //Iterate thru the input and put each label with its address into the map
  for(dword i = 0; i < input.size(); i++)
  {
    if(input[i][0] == '.' && input[i] != ".data")
    {
      labels[input[i]] = i;
      input.erase(input.begin() + i);
      i--;
    }
  }
}

void print_all_memory(Virtual_memory &mem)
{
  printf("***INSTRUCTION BINARY***\n");
  for(size_t i = 0; i < mem.size; i += 4)
    printf("%-12" PRIu64 "%010" PRIu32 "\t%" PRIX32 "\n", i, *((word *)(mem.data + i)), *((word *)(mem.data + i)));
  printf("***DONE***\n");
}
