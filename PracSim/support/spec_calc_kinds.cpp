//
//  File = spec_calc_kinds.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "parmfile.h"
#include "spect_calc_kinds.h"
#include "psstream.h"
extern ParmFile *ParmInput;

//======================================================

KIND_OF_SPECT_CALC_T GetKindOfSpectCalcParm(const char* parm_nam)
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

  if(!strcmp(parm_str,"SPECT_CALC_SAMPLE_SPECTRUM")) return(SPECT_CALC_SAMPLE_SPECTRUM);
  if(!strcmp(parm_str,"SPECT_CALC_BARTLETT_PDGM")) return(SPECT_CALC_BARTLETT_PDGM);
  ErrorStream <<  "Error: '" << parm_str 
              << "' is not a legal value for type KIND_OF_SPECT_CALC_T" << endl;
  exit(-1);
}
ostream& operator<<( ostream& s, const KIND_OF_SPECT_CALC_T& kind_spect_calc)
{
  switch (kind_spect_calc)
    {
    case SPECT_CALC_SAMPLE_SPECTRUM:
      s << "SPECT_CALC_SAMPLE_SPECTRUM";
      break;
    case SPECT_CALC_BARTLETT_PDGM:
      s << "SPECT_CALC_BARTLETT_PDGM";
      break;
    default:
      s << "unknown KIND_OF_SPECT_CALC_T";
    } // end of switch on kind_spect_calc
 return s;
}
PracSimStream& operator<<( PracSimStream& s, const KIND_OF_SPECT_CALC_T& kind_spect_calc)
{
  switch (kind_spect_calc)
    {
    case SPECT_CALC_SAMPLE_SPECTRUM:
      s << "SPECT_CALC_SAMPLE_SPECTRUM";
      break;
    case SPECT_CALC_BARTLETT_PDGM:
      s << "SPECT_CALC_BARTLETT_PDGM";
      break;
    default:
      s << "unknown KIND_OF_SPECT_CALC_T";
    } // end of switch on kind_spect_calc
 return s;
}
