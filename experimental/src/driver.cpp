#include <cctype>
#include <fstream>
#include <cassert>
#include<sstream>

#include "driver.h"
#include "asm_global.h"

LS::Driver::~Driver()
{
   delete(lexer);
   lexer = nullptr;
   delete(parser);
   parser = nullptr;
}


void LS::Driver::parse( std::istream &stream )
{
   if( ! stream.good()  && stream.eof() )
   {
       return;
   }
   parse_helper( stream );
   return;
}

bool LS::Driver::parse(std::string & input)
{
  std::istringstream temp(input);
  return parse_helper(temp);
}

void LS::Driver::print_error( const location & l, const std::string & err_message )
{
  FATALERROR("Parsing error at: " << l);
}


bool LS::Driver::parse_helper( std::istream &stream )
{
  if(lexer)
   delete(lexer);
   try
   {
      lexer = new LS::Lexer( &stream );
   }
   catch( std::bad_alloc &ba )
   {
      FATALERROR("Failed to allocate lexer! Are you out of memory?");
      return false;
   }
   if(parser)
    delete(parser);
   try
   {
      parser = new LS::Parser( (*lexer) /* scanner */,
                                  (*this) /* driver */ );
   }
   catch( std::bad_alloc &ba )
   {
     FATALERROR("Failed to allocate parser! Are you out of memory?");
     return false;
   }
   const int accept( 0 );
   if( parser->parse() != accept )
   {
      return false;
   }
   return true;
}
