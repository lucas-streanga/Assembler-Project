#ifndef _ASM_DRIVER_H
#define _ASM_DRIVER_H

#include <string>
#include <cstddef>
#include <istream>

#include "asm_lexer.h"
#include "asm_parser.tab.h"

namespace LS{

class Driver{
public:
   Driver() = default;

   virtual ~Driver();

   void parse(std::istream &iss);
   bool parse(std::string &);

   bool parse_helper( std::istream &stream );

   void print_error(const location & l, const std::string & err_message);

   LS::Parser  *parser  = nullptr;
   LS::Lexer *lexer = nullptr;
};

}

#endif
