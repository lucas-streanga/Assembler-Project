#ifndef _ERROR_HANDLER_H
#define _ERROR_HANDLER_H

#include<iostream>
#include"asm_defines.h"
#include"file_info.h"

#define FATALERROR(x) std::cerr << TERM_COLOR_ERROR << "\n" << x << "\n\nExitting now..." << TERM_COLOR_RESET << '\n'
#define NONFATALERROR(x) std::cerr << TERM_COLOR_WARN << x << TERM_COLOR_RESET << '\n'


/* Prototypes */
void error_handler(word, dword index, file_info *, const char *);

#endif
