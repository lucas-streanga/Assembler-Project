#ifndef _H_ASM_HELPER
#define _H_ASM_HELPER

#include<iostream>
#include<string>
#include<fstream>
#include"asm_global.h"

//Function prototypes for conditional lookup function...
int conditional_lookup(const char[3]);
int opcode_lookup(const char[5]);
void string_to_lower(std::string &);
bool check_cli_args_and_init(int argc, char ** argv, std::ifstream &, std::ofstream &, std::string &);

#endif
