#include"error_handler.h"

void error_handler(word error_code, dword line, const char * str)
{
  switch(error_code)
  {
    case ERR_SLF:
      NONFATALERROR("String literal is not properly formatted.\nString literals should be contained in \"\".\nAt line: " << line);
      FATALERROR(str);
      error_occurred = 1;
      break;
    case ERR_LNN:
      NONFATALERROR("Numeric literal is not a number.\nAt line: " << line);
      FATALERROR(str);
      error_occurred = 1;
      break;
    case ERR_LTL:
      NONFATALERROR("Numeric literal is too large to fit into 32 bit value.\nAt line: " << line);
      FATALERROR(str);
      error_occurred = 1;
      break;
    case ERR_UTA:
      FATALERROR("Internal Error. Unable to allocate memory for the program.\nIs the program too large?");
      error_occurred = 1;
      break;
    case ERR_FOP:
      NONFATALERROR("Operation is not properly formatted.\nOpcodes must end with \':\'.\nAt line: " << line);
      FATALERROR(str);
      error_occurred = 1;
      break;
    case ERR_UOC:
      NONFATALERROR("Unknown opcode.\nAt line: " << line);
      FATALERROR(str);
      error_occurred = 1;
      break;
    case ERR_UCO:
      NONFATALERROR("Unknown condtional.\nAt line: " << line);
      FATALERROR(str);
      error_occurred = 1;
      break;
    case ERR_INA:
      NONFATALERROR("Incorrect number of arguments for operation.\nAt line: " << line);
      FATALERROR(str);
      error_occurred = 1;
      break;
    case ERR_IVP:
      NONFATALERROR("Immediate value or shift not prefixed with \'#\'.\nAt line: " << line);
      FATALERROR(str);
      error_occurred = 1;
      break;
    case ERR_URR:
      NONFATALERROR("Unable to resolve register values.\nRegisters should be prefixed with \'r\' and be between from 0 to 15.\nAt line: " << line);
      FATALERROR(str);
      error_occurred = 1;
      break;
    case ERR_URI:
      NONFATALERROR("Unable to resolve immediate or shift value.\nMake sure they fit in the proper bit size.\nAt line: " << line);
      FATALERROR(str);
      error_occurred = 1;
      break;
    case ERR_NFS:
      FATALERROR("No file specified to output to.");
      error_occurred = 1;
      break;
    case ERR_UOF:
      FATALERROR("Unable to open file " << str << " for output");
      error_occurred = 1;
      break;
  }
}
