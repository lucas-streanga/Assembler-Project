#ifndef _INIT_H
#define _INIT_H

#include<stdio.h>
#include<iostream>
#include<string>
#include<stdint.h>
#include<stdlib.h>
#include<vector>
#include"asm_defines.h"
#include"error_handler.h"
#include"Virtual_memory.h"
#include"file_info.h"
#include<fstream>
#include<map>
#include<iterator>
#include<algorithm>

/* Prototypes */

void inline STRING_TO_LOWER(std::string &);
void inline STRING_RM_WHITESPACE(std::string &);
dword reserve_line_count(std::ifstream & file);

void format_string_literals(std::vector<std::string> &);

void resolve_labels(std::vector<std::string> & input, std::map<std::string, word> & labels);

void get_input(file_info &);
void format(file_info &);
dword get_size(std::vector<std::string> &);
void print_all_memory(Virtual_memory &);

#endif
