//
//  File = filter_types.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "parmfile.h"
#include "filter_types.h"
extern ParmFile *ParmInput;

//======================================================

FILT_BAND_CONFIG_T GetFiltBandConfigParm(const char* parm_nam)
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

  if(!strcmp(parm_str,"LOWPASS_FILT_BAND_CONFIG")) return(LOWPASS_FILT_BAND_CONFIG);
  if(!strcmp(parm_str,"HIGHPASS_FILT_BAND_CONFIG")) return(HIGHPASS_FILT_BAND_CONFIG);
  if(!strcmp(parm_str,"BANDPASS_FILT_BAND_CONFIG")) return(BANDPASS_FILT_BAND_CONFIG);
  if(!strcmp(parm_str,"BANDSTOP_FILT_BAND_CONFIG")) return(BANDSTOP_FILT_BAND_CONFIG);
  if(!strcmp(parm_str,"ALLPASS_FILT_BAND_CONFIG")) return(ALLPASS_FILT_BAND_CONFIG);
  return(INVALID_FILT_BAND_CONFIG);
}
