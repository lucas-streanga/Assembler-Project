%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {LS}
%define api.parser.class {Parser}

%code requires {
   namespace LS
   {
      class Driver;
      class Lexer;
   }

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { Lexer  &lexer  }
%parse-param { Driver  &driver  }

%code{
#include <iostream>
#include <cstdlib>
#include <fstream>

/* include for all driver functions */
#include "driver.h"

#undef yylex
#define yylex lexer.yylex
}

%define api.value.type variant
%define parse.assert

%token               END    0     "end of file"
%token               UPPER
%token               LOWER
%token <std::string> WORD
%token               NEWLINE
%token               CHAR

%locations
%require "3.2"

%%

list_option : END | list END;

list
  : item
  | list item
  ;
item
  : WORD

%%


void
LS::Parser::error( const location_type & l, const std::string & err_message )
{
   driver.print_error(l, err_message);
}
