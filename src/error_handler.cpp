#include"error_handler.h"

void error_handler(word error_code, dword index, file_info * info, const char * message)
{
  dword line;
  std::string line_from_file;
  if(info != NULL)
  {
    if(info->real_lines != NULL)
    {
      line = info->real_lines[index];
      info->input_file->clear();
      info->input_file->seekg(0);
      //We need to find the actual line from the file...
      for(dword i = 0; i < line; i++)
      {
        getline(*(info->input_file), line_from_file);
        LOG(line_from_file);
      }
    }
  }

  switch(error_code)
  {
    case ERR_SLF:
      NONFATALERROR("String literal is not properly formatted.\nString literals should be contained in \"\".\nAt line: " << line);
      FATALERROR(line_from_file);
      break;
    case ERR_LNN:
      NONFATALERROR("Numeric literal is not a number.\nAt line: " << line);
      FATALERROR(line_from_file);
      break;
    case ERR_LTL:
      NONFATALERROR("Numeric literal is too large to fit into 32 bit value.\nAt line: " << line);
      FATALERROR(line_from_file);
      break;
    case ERR_UTA:
      FATALERROR("Internal Error. Unable to allocate memory for the program.\nIs the program too large?");
      break;
    case ERR_FOP:
      NONFATALERROR("Operation is not properly formatted.\nOpcodes must end with \':\'.\nAt line: " << line);
      FATALERROR(line_from_file);
      break;
    case ERR_UOC:
      NONFATALERROR("Unknown opcode.\nAt line: " << line);
      FATALERROR(line_from_file);
      break;
    case ERR_UCO:
      NONFATALERROR("Unknown condtional.\nAt line: " << line);
      FATALERROR(line_from_file);
      break;
    case ERR_INA:
      NONFATALERROR("Incorrect number of arguments for operation.\nAt line: " << line);
      FATALERROR(line_from_file);
      break;
    case ERR_IVP:
      NONFATALERROR("Immediate value or shift not prefixed with \'#\'.\nAt line: " << line);
      FATALERROR(line_from_file);
      break;
    case ERR_URR:
      NONFATALERROR("Unable to resolve register values.\nRegisters should be prefixed with \'r\' and be between from 0 to 15.\nAt line: " << line);
      FATALERROR(line_from_file);
      break;
    case ERR_URI:
      NONFATALERROR("Unable to resolve immediate or shift value.\nMake sure they fit in the proper bit size.\nAt line: " << line);
      FATALERROR(line_from_file);
      break;
    case ERR_NFS:
      FATALERROR("No file specified for input.");
      break;
    case ERR_UOF:
      FATALERROR("Unable to open file " << message << ".");
      break;
    case ERR_UKL:
      NONFATALERROR("Unknown label at line: " << line);
      FATALERROR(line_from_file);
      break;
  }
  error_occurred = 1;
}
