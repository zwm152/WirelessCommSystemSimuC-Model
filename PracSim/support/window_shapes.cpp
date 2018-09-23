//
//  File = window_shapes.cpp
//

#include <stdlib.h>
//#include <fstream>
#include <string.h>
#include "parmfile.h"
#include "window_shapes.h"
#include "psstream.h"
extern ParmFile *ParmInput;

//======================================================

WINDOW_SHAPE_T GetWindowShapeParm(const char* parm_nam)
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

  if(!strcmp(parm_str,"WINDOW_SHAPE_TRIANGULAR")) return(WINDOW_SHAPE_TRIANGULAR);
  if(!strcmp(parm_str,"WINDOW_SHAPE_HAMMING")) return(WINDOW_SHAPE_HAMMING);
  if(!strcmp(parm_str,"WINDOW_SHAPE_HANN")) return(WINDOW_SHAPE_HANN);
  ErrorStream <<  "Error: '" << parm_str 
              << "' is not a legal value for type WINDOW_SHAPE_T" << endl;
  exit(-1);
}
ostream& operator<<( ostream& s, const WINDOW_SHAPE_T& win_shape_val)
{
  switch (win_shape_val)
    {
    case WINDOW_SHAPE_TRIANGULAR:
      s << "WINDOW_SHAPE_TRIANGULAR";
      break;
    case WINDOW_SHAPE_HAMMING:
      s << "WINDOW_SHAPE_HAMMING";
      break;
    case WINDOW_SHAPE_HANN:
      s << "WINDOW_SHAPE_HANN";
      break;
    default:
      s << "unknown WINDOW_SHAPE_T";
    } // end of switch on win_shape_val
 return s;
}
PracSimStream& operator<<( PracSimStream& s, const WINDOW_SHAPE_T& win_shape_val)
{
  switch (win_shape_val)
    {
    case WINDOW_SHAPE_TRIANGULAR:
      s << "WINDOW_SHAPE_TRIANGULAR";
      break;
    case WINDOW_SHAPE_HAMMING:
      s << "WINDOW_SHAPE_HAMMING";
      break;
    case WINDOW_SHAPE_HANN:
      s << "WINDOW_SHAPE_HANN";
      break;
    default:
      s << "unknown WINDOW_SHAPE_T";
    } // end of switch on win_shape_val
 return s;
}
