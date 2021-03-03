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
#include<fstream>
#include<map>

/* Prototypes */

void inline STRING_TO_LOWER(std::string &);
void inline STRING_RM_WHITESPACE(std::string &);

void format_string_literals(std::vector<std::string> &);

void resolve_labels(std::vector<std::string> & input, std::map<std::string, word> & labels);

void get_input(std::vector<std::string> &);
void format(std::vector<std::string> &);
dword get_size(std::vector<std::string> &);
void print_all_memory(Virtual_memory &);

#endif
