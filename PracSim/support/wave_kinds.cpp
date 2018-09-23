//
//  File = wave_kinds.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "parmfile.h"
#include "wave_kinds.h"
#include "psstream.h"
extern ParmFile *ParmInput;

//======================================================

PCM_WAVE_KIND_T GetWaveKindParm(const char* parm_nam)
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

  if(!strcmp(parm_str,"PCM_NRZ")) return(PCM_NRZ);
  if(!strcmp(parm_str,"PCM_BIPHASE")) return(PCM_BIPHASE);
  if(!strcmp(parm_str,"PCM_DELAY_MOD")) return(PCM_DELAY_MOD);
  ErrorStream <<  "Error: '" << parm_str 
              << "' is not a legal value for type PCM_WAVE_KIND_T" << endl;
  exit(-1);
}
ostream& operator<<( ostream& s, const PCM_WAVE_KIND_T& dly_mode_val)
{
  switch (dly_mode_val)
    {
    case PCM_NRZ:
      s << "PCM_NRZ";
      break;
    case PCM_BIPHASE:
      s << "PCM_BIPHASE";
      break;
    case PCM_DELAY_MOD:
      s << "PCM_DELAY_MOD";
      break;
    default:
      s << "unknown PCM_WAVE_KIND_T";
    } // end of switch on wave_kind_val
 return s;
}
PracSimStream& operator<<( PracSimStream& s, const PCM_WAVE_KIND_T& wave_kind_val)
{
  switch (wave_kind_val)
    {
    case PCM_NRZ:
      s << "PCM_NRZ";
      break;
    case PCM_BIPHASE:
      s << "PCM_BIPHASE";
      break;
    case PCM_DELAY_MOD:
      s << "PCM_DELAY_MOD";
      break;
    default:
      s << "unknown PCM_WAVE_KIND_T";
    } // end of switch on wave_kind_val
 return s;
}
