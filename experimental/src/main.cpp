#include"helper.h"
#include"asm_global.h"
#include"lookup_tables.h"
#include<iostream>
#include<fstream>
#include"driver.h"


struct string_input : public std::streambuf
{
    string_input(char* s, std::size_t n)
    {
        setg(s, s, s + n);
    }
};


int main(int argc, char ** argv)
{

  std::cout << "Conditional LS" << conditional_lookup("ls") << '\n';
  std::cout << "Opcode prnm" << opcode_lookup("end") << '\n';

  //Input and output file...
  std::ofstream out_file;
  std::ifstream in_file;

  //Input will just be stored in a string
  std::string input;

  //Check the inputs and init the files and input string
  if(!check_cli_args_and_init(argc, argv, in_file, out_file, input))
    return -1;


  //Log the input
  LOG("Entire input file: \n=====START OF FILE=====\n" << input << "\n=====END OF FILE=====\n\n");

  LOG("Generating inital lookup tables...");

  LS::Driver d;

  if(!d.parse(input));
    return 1;

  return 0;
}
