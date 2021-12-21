#ifndef _ASM_LEXER
#define _ASM_LEXER

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "asm_parser.tab.h"
#include "location.hh"

namespace LS{

class Lexer : public yyFlexLexer{
public:

   Lexer(std::istream *in) : yyFlexLexer(in)
   {
      loc = new LS::Parser::location_type();
   };

   ~Lexer()
   {
      delete loc;
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual
   int yylex( LS::Parser::semantic_type * const lval,
              LS::Parser::location_type *location );
   // YY_DECL defined in mc_lexer.l
   // Method body created by flex in mc_lexer.yy.cc


private:
   /* yyval ptr */
   LS::Parser::semantic_type *yylval = nullptr;
   /* location ptr */
   LS::Parser::location_type *loc    = nullptr;
};

}

#endif
