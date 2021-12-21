#include"decode.h"
#include<sstream>
#include<vector>
#include<string>
#include<cstring>

word get_data(const std::string & s)
{
  //We need to see if each char is a number.
  //If so, convert to a number.

  bool is_number = true;
  bool is_decimal = false;
  word ret = 0;

  if(s[0] == ',')
  {
    for(dword i = 1; i < s.size(); i++)
      ret = ret | (s[i] << ((i-1) * 8));

    return ret;
  }

  for(dword i = 0; i < s.size(); i++)
  {
    if(!isdigit(s[i]))
      is_number = false;
    else if(s[i] == '.')
      is_decimal = true;
  }

  if(is_number && !is_decimal)
  {
    try
    {
      ret = stoll(s) & MAX_U;
    }
    catch(...)
    {
      ret = 0;
    }
  }
  else if(is_decimal)
  {
    try
    {
      ret = stof(s);
    }
    catch(...)
    {
      ret = 0;
    }
  }

  return ret;

}

word get_instruction(file_info & info, const std::string & s, word line, std::map<std::string, word> & labels)
{
  //We need to do string parsing to get the instruction.
  //First, we need to find out if the delimiter is there
  //If not, end assembly
  char delim = ':';
  char s_flag = 0;
  std::string conditional;
  std::string opcode;

  if(s.find(delim) == std::string::npos)
    error_handler(ERR_FOP, line, &info, NULL);
  CHK_ERR;

  const std::string & token = s.substr(0, s.find(delim));
  const std::string  & rest = s.substr(s.find(delim) + 1, s.size());

  //We need to get the condtionals and the s_flag
  //3 is no condtional or condtionals
  if(token.size() == 3)
  {
    conditional = "al";
    s_flag = 0;
    opcode = token;
  }
  else if(token.size() == 4)
  {
    conditional = "al";
    s_flag = token[3];
    opcode = token.substr(0, 3);
  }
  else if(token.size() == 5)
  {
    conditional = token.substr(0, 2);
    s_flag = 0;
    opcode = token.substr(2, 3);
  }
  else if(token.size() == 6)
  {
    conditional = token.substr(0, 2);
    s_flag = token[5];
    opcode = token.substr(2, 3);
  }

  byte op, cond, sbit;

  op = str_to_op(info, opcode, s_flag, line);
  CHK_ERR;
  cond = str_to_cond(info, conditional, line);
  CHK_ERR;
  sbit = str_to_s(s_flag);

  if(op == OP_CMP)
    sbit = 1;

  LOG("Opcode " << opcode << " " << (int) op);
  LOG("Conditional " << conditional << " " << (int) cond);
  LOG("S set? " << (int) sbit);

  word ret = decode(info, op, cond, sbit, rest, s, line, labels);
  return ret;
}

word decode(file_info & info, byte op, byte cond, byte s, const std::string& rest, const std::string& ins, word line, std::map<std::string, word> & labels)
{
  word ret;
  if(op <= 1)
    ret = op_addsub(info, op, cond, s, rest, ins, line);
  else if(op <= 9)
    ret = op_ldrstr(info, op, cond, s, rest, ins, line, labels);
  else if(op == 10)
    ret = op_prnr(info, op, cond, s, rest, ins, line);
  else if(op == 11)
    ret = op_prnm(info, op, cond, s, rest, ins, line);
  else if(op <= 13)
    ret = op_brn(info, op, cond, s, rest, ins, line, labels);
  else if(op == OP_END)
    ret = MAX_U;

  return ret;
}

word op_prnm(file_info & info, byte op, byte cond, byte s, const std::string& rest, const std::string& ins, word line)
{
  /* shift is 10 bits with 4 bit reg */
  /* or immed value of 14 bits */

  //We need to use reserve smartly to avoid extra heap alloc
  dword Rn, specifier = 0, num_bytes = 0, shift = 0, immed = 0;
  std::string spec;
  std::string nbytes;
  byte I = 0;
  std::stringstream ss(rest);
  std::string arguments[4];
  byte num_args = 0;

  //We need to break this up with delimiting commas
  while(ss.good() && num_args <= 4)
  {
    std::string str;
    getline(ss, str, ',');
    if(!str.empty())
    {
      arguments[num_args] = str;
      num_args++;
    }
  }
  for(byte i = 0; i < num_args; i++)
    LOG(arguments[i]);
  if(num_args < 3 || num_args > 4)
    error_handler(ERR_INA, line, &info, NULL);

  CHK_ERR;

  if(arguments[0][0] == '#')
    I = 1;

  for(byte i = 0; i < num_args; i++)
    if(arguments[i][0] == 'r' || arguments[i][0] == '#')
      arguments[i].erase(0, 1);

  spec = arguments[num_args - 1];
  if(spec == "uint")
    specifier = 0;
  else if(spec == "int")
    specifier = 1;
  else if(spec == "hex")
    specifier = 2;
  else if(spec == "char")
    specifier = 3;
  else
    error_handler(ERR_INA, line, &info, NULL);
  CHK_ERR;

  nbytes = arguments[num_args - 2];
  if(nbytes == "byte")
    num_bytes = 0;
  else if(nbytes == "half")
    num_bytes = 1;
  else if(nbytes == "word")
    num_bytes = 2;
  else if(nbytes == "dword")
    num_bytes = 3;
  else
    error_handler(ERR_INA, line, &info, NULL);
  CHK_ERR;

  if(num_args == 3 && I)
  {
    try
    {
      immed = stoll(arguments[0]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    if(immed > PRNM_IMMED_MAX)
      error_handler(ERR_URI, line, &info, NULL);
      CHK_ERR;
  }
  else if(num_args == 3 && !I) //No immediate value, op2 is a reg
  {
    try
    {
      Rn = stoll(arguments[0]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    if(Rn > 15)
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
  }
  else  //Shift, Rd, Rn, and Op2 are regs and last is shift
  {
    try
    {
      Rn = stoll(arguments[0]);
      shift = stoll(arguments[2]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    if(Rn > 15)
      error_handler(ERR_URR, line, &info, NULL);
    CHK_ERR;
    if(shift > PRNM_SHIFT_MAX)
      error_handler(ERR_URI, line, &info, NULL);
    CHK_ERR;
  }

  LOG("Instruction Decoded:");
  LOG("I? " << (int) I);
  LOG("Num bytes: " << (int) num_bytes);
  if(I)
    LOG("Immediate value: " << (int) immed);
  else
    LOG("Rn: " << (int) Rn << "\nShift: " << (int) shift);

  word ret;
  if(I)
    ret = (op << 24) | (cond << 20) | (I << 19) | (s << 18) | (num_bytes << 16) | (immed << 2) | (specifier);
  if(!I)
    ret = (op << 24) | (cond << 20) | (I << 19) | (s << 18) | (num_bytes << 16) | (Rn << 12) | (shift << 2) | (specifier);

  LOG("INSTRUCTION:\n" << (unsigned long int) ret);
  return ret;

}

word op_prnr(file_info & info, byte op, byte cond, byte s, const std::string& rest, const std::string& ins, word line)
{
  dword num_registers, specifier;
  word R[4] = {0};
  dword Rn;
  std::string spec;
  std::stringstream ss(rest);
  std::string arguments[5];
  byte num_args = 0;

  //We need to break this up with delimiting commas
  while(ss.good() && num_args <= 5)
  {
    std::string str;
    getline(ss, str, ',');
    if(!str.empty())
    {
      arguments[num_args] = str;
      num_args++;
    }
  }
  for(byte i = 0; i < num_args; i++)
    LOG(arguments[i]);
  if(num_args < 2 || num_args > 5)
    error_handler(ERR_INA, line, &info, NULL);
  CHK_ERR;

  num_registers = num_args - 1;
  spec = arguments[num_args - 1];
  if(spec == "uint")
    specifier = 0;
  else if(spec == "int")
    specifier = 1;
  else if(spec == "hex")
    specifier = 2;
  else if(spec == "char")
    specifier = 3;
  else
    error_handler(ERR_INA, line, &info, NULL);
  CHK_ERR;

  for(byte i = 0; i < num_args; i++)
    if(arguments[i][0] == 'r')
      arguments[i].erase(0, 1);

  for(byte i = 0; i < num_args - 1; i++)
  {
    try
    {
      Rn = stoll(arguments[i]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    if(Rn > 15)
      error_handler(ERR_URR, line, &info, NULL);
    CHK_ERR;

    R[i] = Rn;
  }

  LOG("INSTRUCTION DECODED");
  LOG("Number of registers to print: " << (int) num_registers);
  LOG("Reg 1 " << (int) R[0]);
  LOG("Reg 2 " << (int) R[1]);
  LOG("Reg 3 " << (int) R[2]);
  LOG("Reg 4 " << (int) R[3]);
  LOG("Specifier " << (int) specifier);


  word ret;
  ret = (op << 24) | (cond << 20) | ((num_registers - 1) << 18) | (R[0] << 14) | (R[1] << 10) | (R[2] << 6) | (R[3] << 2) | specifier;
  LOG("INSTRUCTION:\n" << (unsigned long int) ret);
  return ret;

}

word op_ldrstr(file_info & info, byte op, byte cond, byte s, const std::string& rest, const std::string& ins, word line, std::map<std::string, word> & labels)
{
  dword Rd, Rn, immed, shift = 0;
  byte I = 0;
  byte has_label = 0;

  std::stringstream ss(rest);
  std::vector<std::string> arguments;
  byte num_args = 0;

  //We need to break this up with delimiting commas
  while(ss.good())
  {
    std::string str;
    getline(ss, str, ',');
    if(!str.empty())
    {
      arguments.push_back(str);
      num_args++;
    }
  }
  for(byte i = 0; i < num_args; i++)
    LOG(arguments[i]);
  if(num_args < 2 || num_args > 3)
    error_handler(ERR_INA, line, &info, NULL);
  CHK_ERR;
  if(arguments[1][0] == '#')
    I = 1;

  for(byte i = 0; i < num_args; i++)
  {
    if(arguments[i][0] == 'r' || arguments[i][0] == '#')
      arguments[i].erase(0, 1);
    else if (i == 2 && arguments[i][0] == '.')
      has_label = 1;
    else
      error_handler(ERR_URI, line, &info, NULL);
    CHK_ERR;
  }

  if(num_args == 2 && I)
  {
    try
    {
      Rd = stoll(arguments[0]);
      immed = stoll(arguments[1]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    if(Rd > 15)
      error_handler(ERR_URR, line, &info, NULL);
    CHK_ERR;
    if(immed > LS_IMMED_MAX)
      error_handler(ERR_URI, line, &info, NULL);
    CHK_ERR;
  }
  else if(num_args == 2 && !I) //No immediate value, op2 is a reg
  {
    try
    {
      Rd = stoll(arguments[0]);
      Rn = stoll(arguments[1]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    if(Rd > 15 || Rn > 15)
      error_handler(ERR_URR, line, &info, NULL);
    CHK_ERR;
  }
  else  //Shift, Rd, Rn, and Op2 are regs and last is shift
  {
    try
    {
      Rd = stoll(arguments[0]);
      Rn = stoll(arguments[1]);
      if(has_label)
      {
        LOG("Label in use");
        std::map<std::string, word>::iterator it;
        it = labels.find(arguments[2]);
        if(it == labels.end())
          error_handler(ERR_UKL, line, &info, NULL);
        CHK_ERR;

        //Compute the shift from the stored value in the label map
        shift = labs((long int) (it->second) * 4);
        LOG("Shift: " << (int) shift);
      }
      else
        shift = stoll(arguments[2]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    if(Rd > 15 || Rn > 15)
      error_handler(ERR_URR, line, &info, NULL);
    CHK_ERR;
    if(shift > LS_SHIFT_MAX)
      error_handler(ERR_URI, line, &info, NULL);
    CHK_ERR;
  }

  LOG("Instruction Decoded:");
  LOG("I? " << (int) I);
  LOG("S? " << (int) s);
  LOG("RD: " << (int) Rd);
  if(I)
    LOG("Immediate value: " << (int) immed);
  else
    LOG("Reg Rn: " << (int) Rn << "\nShift: " << (int) shift);

  word ret;
  if(I)
    ret = (op << 24) | (cond << 20) | (I << 19) | (s << 18) | (Rd << 14) | (immed);
  if(!I)
    ret = (op << 24) | (cond << 20) | (I << 19) | (s << 18) | (Rd << 14) | (Rn << 10) | (shift);


  LOG("INSTRUCTION:\n" << (unsigned long int) ret);
  return ret;
}

word op_addsub(file_info & info, byte op, byte cond, byte s, const std::string& rest, const std::string& ins, word line)
{
  dword Rd, Rn, op2, shift = 0;
  byte I = 0;
  std::stringstream ss(rest);
  std::string arguments[4];
  byte num_args = 0;
  //We need to break this up with delimiting commas
  while(ss.good() && num_args <= 4)
  {
    std::string str;
    getline(ss, str, ',');
    if(!str.empty())
    {
      arguments[num_args] = str;
      num_args++;
    }
  }
  for(byte i = 0; i < num_args; i++)
    LOG(arguments[i]);

  if(num_args < 3 || num_args > 4)
    error_handler(ERR_INA, line, &info, NULL);
  CHK_ERR;
  if(arguments[2][0] == '#')
    I = 1;

  for(byte i = 0; i < num_args; i++)
  {
    if(arguments[i][0] == 'r' || arguments[i][0] == '#')
      arguments[i].erase(0, 1);
    else
      error_handler(ERR_URI, line, &info, NULL);
    CHK_ERR;
  }

  //Three args, third one being immediate value
  if(num_args == 3 && I)
  {
    try
    {
      Rd = stoll(arguments[0]);
      Rn = stoll(arguments[1]);
      op2 = stoll(arguments[2]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    if(Rd > 15 || Rn > 15)
      error_handler(ERR_URR, line, &info, NULL);
    CHK_ERR;
    if(op2 > AS_IMMED_MAX)
      error_handler(ERR_URI, line, &info, NULL);
    CHK_ERR;
  }
  else if(num_args == 3 && !I) //No immediate value, op2 is a reg
  {
    try
    {
      Rd = stoll(arguments[0]);
      Rn = stoll(arguments[1]);
      op2 = stoll(arguments[2]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    if(Rd > 15 || Rn > 15 || op2 > 15)
      error_handler(ERR_URR, line, &info, NULL);
    CHK_ERR;
  }
  else  //Shift, Rd, Rn, and Op2 are regs and last is shift
  {
    try
    {
      Rd = stoll(arguments[0]);
      Rn = stoll(arguments[1]);
      op2 = stoll(arguments[2]);
      shift = stoll(arguments[3]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    if(Rd > 15 || Rn > 15 || op2 > 15)
      error_handler(ERR_URR, line, &info, NULL);
    CHK_ERR;
    if(shift > AS_SHIFT_MAX)
      error_handler(ERR_URI, line, &info, NULL);
    CHK_ERR;
  }

  LOG("Instruction Decoded:");
  LOG("I? " << (int) I);
  LOG("S? " << (int) s);
  LOG("RD: " << (int) Rd);
  LOG("RN: " << (int) Rn);
  if(I)
    LOG("Immediate value: " << (int) op2);
  else
    LOG("Reg op2: " << (int) op2 << "\nShift: " << (int) shift);

  word ret;
  if(I)
    ret = (op << 24) | (cond << 20) | (I << 19) | (s << 18) | (Rd << 14) | (Rn << 10) | (op2);
  if(!I)
    ret = (op << 24) | (cond << 20) | (I << 19) | (s << 18) | (Rd << 14) | (Rn << 10) | (op2 << 6) | (shift);


  LOG("INSTRUCTION:\n" << (unsigned long int) ret);
  return ret;

}

word op_brn(file_info & info, byte op, byte cond, byte s, const std::string& rest, const std::string& ins, word line, std::map<std::string, word> & labels)
{
  dword shift = 0;
  byte neg = 0;
  std::stringstream ss(rest);
  std::string arguments[1];
  byte num_args = 0;

  //We need to break this up with delimiting commas
  while(ss.good() && num_args <= 1)
  {
    std::string str;
    getline(ss, str, ',');
    if(!str.empty())
    {
      arguments[num_args] = str;
      num_args++;
    }
  }
  for(byte i = 0; i < num_args; i++)
    LOG(arguments[i]);

  if(num_args != 1)
    error_handler(ERR_INA, line, &info, NULL);
  CHK_ERR;

  //If it is a label, we need to convert the label to a literal value.
  if(arguments[0][0] == '.')
  {
    LOG("Label in use");
    std::map<std::string, word>::iterator it;
    it = labels.find(arguments[0]);
    if(it == labels.end())
      error_handler(ERR_UKL, line, &info, NULL);
    CHK_ERR;

    //Compute the shift from the stored value in the label map
    shift = labs((long int) line - (long int) it->second);
    if(it->second < line)
      neg = 1;
  }
  else if(arguments[0][0] == '#')
  {
    arguments[0].erase(0, 1);

    if(arguments[0][0] == '-')
    {
      neg = 1;
      arguments[0].erase(0, 1);
    }

    try
    {
      shift = stoll(arguments[0]);
    }
    catch(const std::invalid_argument & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
    catch(const std::out_of_range & e)
    {
      error_handler(ERR_URR, line, &info, NULL);
      CHK_ERR;
    }
  }
  if(shift > BRN_SHIFT_MAX)
    error_handler(ERR_URI, line, &info, NULL);
  CHK_ERR;

  LOG("Instruction Decoded:");
  LOG("Negative? " << (int) neg);
  LOG("Offset: " << (unsigned int) shift);

  word ret = (op << 24) | (cond << 20) | (neg << 19) | (shift);

  LOG("INSTRUCTION:\n" << (unsigned long int) ret);
  return ret;
}

byte str_to_op(file_info & info, const std::string & opcode, char s_flag, word line)
{
  if(opcode == "add")
    return OP_ADD;
  else if(opcode == "sub")
    return OP_SUB;
  else if(opcode == "ldr" && s_flag == 0)
    return OP_LDR;
  else if(opcode == "ldr" && s_flag == 'h')
    return OP_LDRH;
  else if(opcode == "ldr" && s_flag == 'b')
    return OP_LDRB;
  else if(opcode == "str" && s_flag == 0)
    return OP_STR;
  else if(opcode == "str" && s_flag == 'h')
    return OP_STRH;
  else if(opcode == "str" && s_flag == 'b')
    return OP_STRB;
  else if(opcode == "mov")
    return OP_MOV;
  else if(opcode == "cmp")
    return OP_CMP;
  else if(opcode == "prn" && s_flag == 'r')
    return OP_PRNR;
  else if(opcode == "prn" && s_flag == 'm')
    return OP_PRNM;
  else if(opcode == "brn" && s_flag == 0)
    return OP_BRN;
  else if(opcode == "brn" && s_flag == 'l')
    return OP_BRNL;
  else if(opcode == "end")
    return OP_END;
  else
    error_handler(ERR_UOC, line, &info, NULL);
  CHK_ERR;

  return 0;
}

byte str_to_cond(file_info & info, const std::string & cond, word line)
{
  if(cond == "al")
    return COND_AL;
  else if(cond == "eq")
    return COND_EQ;
  else if(cond == "ne")
    return COND_NE;
  else if(cond == "cs")
    return COND_CS;
  else if(cond == "cc")
    return COND_CC;
  else if(cond == "mi")
    return COND_MI;
  else if(cond == "pl")
    return COND_PL;
  else if(cond == "os")
    return COND_OS;
  else if(cond == "oc")
    return COND_OC;
  else if(cond == "hi")
    return COND_HI;
  else if(cond == "ls")
    return COND_LS;
  else if(cond == "ge")
    return COND_GE;
  else if(cond == "lt")
    return COND_LT;
  else if(cond == "gt")
    return COND_GT;
  else if(cond == "le")
    return COND_LE;
  else if(cond == "nv")
    return COND_NV;
  else
    error_handler(ERR_UCO, line, &info, NULL);
  CHK_ERR;

  return 0;
}

byte str_to_s(char s)
{
  if(s == 's')
    return 1;
  return 0;
}
