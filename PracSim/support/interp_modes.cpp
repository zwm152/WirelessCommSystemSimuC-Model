//
//  File = interp_modes.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "parmfile.h"
#include "interp_modes.h"
#include "psstream.h"
extern ParmFile *ParmInput;

//======================================================

INTERP_MODE_T GetInterpModeParm(const char* parm_nam)
{
  char parm_str[30];

  if(ParmInput->GetParmStr(parm_nam, parm_str)!=0)
    {
    ParmInput->RestartBlock();
    if(ParmInput->GetParmStr(parm_nam, parm_str) !=0)
      {
      ErrorStream <<  "Error: parameter '" << parm_nam 
                  << "' not found after 2 attempts" << endl;
      exit(-1);
      }
    }

  if(!strcmp(parm_str,"INTERP_MODE_LINEAR")) return(INTERP_MODE_LINEAR);
  if(!strcmp(parm_str,"INTERP_MODE_SINC")) return(INTERP_MODE_SINC);
  return(INTERP_MODE_NONE);
}
ostream& operator<<( ostream& s, const INTERP_MODE_T& interp_mode_val)
{
  switch (interp_mode_val)
    {
    case INTERP_MODE_NONE:
      s << "INTERP_MODE_NONE";
      break;
    case INTERP_MODE_LINEAR:
      s << "INTERP_MODE_LINEAR";
      break;
    case INTERP_MODE_SINC:
      s << "INTERP_MODE_SINC";
      break;
    default:
      s << "unknown INTERP_MODE_T";
    } // end of switch on interp_mode_val
 return s;
}
PracSimStream& operator<<( PracSimStream& s, const INTERP_MODE_T& interp_mode_val)
{
  switch (interp_mode_val)
    {
    case INTERP_MODE_NONE:
      s << "INTERP_MODE_NONE";
      break;
    case INTERP_MODE_LINEAR:
      s << "INTERP_MODE_LINEAR";
      break;
    case INTERP_MODE_SINC:
      s << "INTERP_MODE_SINC";
      break;
    default:
      s << "unknown INTERP_MODE_T";
    } // end of switch on interp_mode_val
 return s;
}
