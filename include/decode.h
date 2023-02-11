#ifndef _DECODE_H
#define _DECODE_H

#include"asm_defines.h"
#include"error_handler.h"
#include"file_info.h"
#include<map>

word get_instruction(file_info &, const std::string &, word line, std::map<std::string, word> & );

word get_data(const std::string &);

byte str_to_op(file_info &, const std::string &, char s_flag, word line);
byte str_to_cond(file_info &, const std::string &, word line);
byte str_to_s(char);

word decode(file_info &, byte op, byte cond, byte s, const std::string &, const std::string &, word, std::map<std::string, word> & );

/*<<<<<<< HEAD
word op_addsub(file_info &, byte op, byte cond, byte s, const std::string &, const std::string &, word);
word op_ldrstr(file_info &, byte op, byte cond, byte s, const std::string &, const std::string &, word, std::map<std::string, word> & );
=======*/
word op_ret(file_info &, byte op, byte cond, byte s, const std::string &, const std::string &, word);
word op_arithmetic_logic(file_info &, byte op, byte cond, byte s, const std::string &, const std::string &, word);
word op_ldrstr(file_info &, byte op, byte cond, byte s, const std::string &, const std::string &, word, std::map<std::string, word> & );
word op_prnr(file_info &, byte op, byte cond, byte s, const std::string &, const std::string &, word);
word op_prnm(file_info &, byte op, byte cond, byte s, const std::string &, const std::string &, word);
word op_brn(file_info &, byte op, byte cond, byte s, const std::string &, const std::string &, word, std::map<std::string, word> & );

#endif
