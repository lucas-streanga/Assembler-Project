#ifndef _DECODE_H
#define _DECODE_H

#include"asm_defines.h"
#include"error_handler.h"
#include<map>

word get_instruction(const std::string &, word line, std::map<std::string, word> & );

word get_data(const std::string &);

byte str_to_op(const std::string &, char s_flag, word line);
byte str_to_cond(const std::string &, word line);
byte str_to_s(char, word line);

word decode(byte op, byte cond, byte s, const std::string &, const std::string &, word, std::map<std::string, word> & );

word op_addsub(byte op, byte cond, byte s, const std::string &, const std::string &, word);
word op_ldrstr(byte op, byte cond, byte s, const std::string &, const std::string &, word);
word op_prnr(byte op, byte cond, byte s, const std::string &, const std::string &, word);
word op_prnm(byte op, byte cond, byte s, const std::string &, const std::string &, word);
word op_brn(byte op, byte cond, byte s, const std::string &, const std::string &, word, std::map<std::string, word> & );

#endif
