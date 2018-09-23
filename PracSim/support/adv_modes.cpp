//
//  File = adv_modes.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "parmfile.h"
#include "adv_modes.h"
#include "psstream.h"
extern ParmFile *ParmInput;

//======================================================

ADVANCE_MODE_T GetAdvanceModeParm(const char* parm_nam)
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

  if(!strcmp(parm_str,"ADVANCE_MODE_NONE")) return(ADVANCE_MODE_NONE);
  if(!strcmp(parm_str,"ADVANCE_MODE_FIXED")) return(ADVANCE_MODE_FIXED);
  if(!strcmp(parm_str,"ADVANCE_MODE_DYNAMIC")) return(ADVANCE_MODE_DYNAMIC);
  if(!strcmp(parm_str,"ADVANCE_MODE_GATED")) return(ADVANCE_MODE_GATED);
  ErrorStream <<  "Error: '" << parm_str 
              << "' is not a legal value for type ADVANCE_MODE_T" << endl;
  exit(-1);
}
ostream& operator<<( ostream& s, const ADVANCE_MODE_T& dly_mode_val)
{
  switch (dly_mode_val)
    {
    case ADVANCE_MODE_NONE:
      s << "ADVANCE_MODE_NONE";
      break;
    case ADVANCE_MODE_FIXED:
      s << "ADVANCE_MODE_FIXED";
      break;
    case ADVANCE_MODE_DYNAMIC:
      s << "ADVANCE_MODE_DYNAMIC";
      break;
    case ADVANCE_MODE_GATED:
      s << "ADVANCE_MODE_GATED";
      break;
    default:
      s << "unknown ADVANCE_MODE_T";
    } // end of switch on dly_mode_val
 return s;
}
PracSimStream& operator<<( PracSimStream& s, const ADVANCE_MODE_T& adv_mode_val)
{
  switch (adv_mode_val)
    {
    case ADVANCE_MODE_NONE:
      s << "ADVANCE_MODE_NONE";
      break;
    case ADVANCE_MODE_FIXED:
      s << "ADVANCE_MODE_FIXED";
      break;
    case ADVANCE_MODE_DYNAMIC:
      s << "ADVANCE_MODE_DYNAMIC";
      break;
    case ADVANCE_MODE_GATED:
      s << "ADVANCE_MODE_GATED";
      break;
    default:
      s << "unknown ADVANCE_MODE_T";
    } // end of switch on adv_mode_val
 return s;
}
