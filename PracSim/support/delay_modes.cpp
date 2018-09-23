//
//  File = delay_modes.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "parmfile.h"
#include "delay_modes.h"
#include "psstream.h"
extern ParmFile *ParmInput;

//======================================================

DELAY_MODE_T GetDelayModeParm(const char* parm_nam)
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

  if(!strcmp(parm_str,"DELAY_MODE_NONE")) return(DELAY_MODE_NONE);
  if(!strcmp(parm_str,"DELAY_MODE_FIXED")) return(DELAY_MODE_FIXED);
  if(!strcmp(parm_str,"DELAY_MODE_DYNAMIC")) return(DELAY_MODE_DYNAMIC);
  if(!strcmp(parm_str,"DELAY_MODE_GATED")) return(DELAY_MODE_GATED);
  ErrorStream <<  "Error: '" << parm_str 
              << "' is not a legal value for type DELAY_MODE_T" << endl;
  exit(-1);
}
ostream& operator<<( ostream& s, const DELAY_MODE_T& dly_mode_val)
{
  switch (dly_mode_val)
    {
    case DELAY_MODE_NONE:
      s << "DELAY_MODE_NONE";
      break;
    case DELAY_MODE_FIXED:
      s << "DELAY_MODE_FIXED";
      break;
    case DELAY_MODE_DYNAMIC:
      s << "DELAY_MODE_DYNAMIC";
      break;
    case DELAY_MODE_GATED:
      s << "DELAY_MODE_GATED";
      break;
    default:
      s << "unknown DELAY_MODE_T";
    } // end of switch on dly_mode_val
 return s;
}
PracSimStream& operator<<( PracSimStream& s, const DELAY_MODE_T& dly_mode_val)
{
  switch (dly_mode_val)
    {
    case DELAY_MODE_NONE:
      s << "DELAY_MODE_NONE";
      break;
    case DELAY_MODE_FIXED:
      s << "DELAY_MODE_FIXED";
      break;
    case DELAY_MODE_DYNAMIC:
      s << "DELAY_MODE_DYNAMIC";
      break;
    case DELAY_MODE_GATED:
      s << "DELAY_MODE_GATED";
      break;
    default:
      s << "unknown DELAY_MODE_T";
    } // end of switch on dly_mode_val
 return s;
}
