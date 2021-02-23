#ifndef _ERROR_HANDLER_H
#define _ERROR_HANDLER_H

#include<iostream>
#include"asm_defines.h"

#define FATALERROR(x) std::cerr << TERM_COLOR_ERROR << "\n" << x << "\n\nExitting now..." << TERM_COLOR_RESET << std::endl
#define NONFATALERROR(x) std::cerr << TERM_COLOR_WARN << x << TERM_COLOR_RESET << std::endl


/* Prototypes */
void error_handler(word, dword line, const char * str);

#endif
