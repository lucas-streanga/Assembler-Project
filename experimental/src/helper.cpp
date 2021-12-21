#include"asm_global.h"
#include"helper.h"
#include<string.h>

void string_to_lower(std::string & s)
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

bool check_cli_args_and_init(int argc, char ** argv, std::ifstream & in_file, std::ofstream & out_file, std::string & input)
{
  //No input file, obviously can't do anything
  if(argc < 2)
  {
    FATALERROR("No file specified for input.");
    return false;
  }
  std::string out_file_name;
  if(argc < 3)
  {
    printf("No output file specified, default %s will be used.\n", DEFAULT_OUT_FILE);
    out_file_name = DEFAULT_OUT_FILE;
  }
  else
  {
    out_file_name = argv[2];
  }

  //Open the files
  out_file.open(out_file_name, std::ios::binary);
  ALLOC_PRINT("This is a file handle: " << out_file_name);
  in_file.open(argv[1]);
  ALLOC_PRINT("This is a file handle: " << argv[1]);

  //Chekc if files opened successfully
  if(!out_file.is_open())
  {
    FATALERROR("Unable to open file " << out_file_name << ".");
    return false;
  }
  if(!in_file.is_open())
  {
    FATALERROR("Unable to open file " << argv[1] << ".");
    return false;
  }
  //Now we can copy over the input file to a string
  std::getline(in_file, input, '\0');
  //make the entire input file lowercase for easier parsing
  string_to_lower(input);
  in_file.close();
  return true;
}

int conditional_lookup(const char conditional[3])
{
  for(int i = 0; i < NUM_CONDITIONALS; i++)
  {
    //If we find the conditional
    if(!strncmp(conditional, COND_LT[i], 2))
      return i;
  }
  return -1;
}

int opcode_lookup(const char opcode[5])
{
  if(!strncmp(opcode, "end", 3))
    return OP_END;

  for(int i = 0; i < NUM_OPCODES; i++)
  {
    if(!strcmp(opcode, OPCODE_LT[i]))
      return i;
  }
  return -1;
}
