//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = unwrap.cpp
//
//

#include <math.h>
//#include <stdlib.h>
//#include <fstream>
#include "unwrap.h"
//using namespace std;

//extern ofstream *DebugFile;

//========================================
//
void UnwrapPhase( int samp_num, double *phase)
{
  static double half_circle_offset;
  static double old_phase;

  if(samp_num==0){
    half_circle_offset = 0.0;
    old_phase = *phase;
    }
  else{
    *phase = *phase + half_circle_offset;
    if( fabs(old_phase - *phase) > double(90.0)){
      if(old_phase < *phase){
        *phase = *phase - 360.0;
        half_circle_offset -= 360.0;
        }
      else{
        *phase = *phase + 360.0;
        half_circle_offset += 360.0;
        }
      }
    old_phase = *phase;
    }
  return;
}
