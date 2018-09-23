//
//  File = rect_pulse_theory.cpp
//

#include <stdlib.h>
#include <math.h>
#include "rect_pulse_theory.h"
#include "sinc.h"
#include <fstream>
#ifndef PI
  #define PI 3.1415926535897932
#endif
using namespace std;

void RectPulseAliasPower( double fold_freq_hz,
                          int samps_per_hz,
                          char* out_filename)
{
   double freq, val;
   ofstream *out_file;
   out_file = new ofstream(out_filename, ios::out);

   int num_pts = int(1 + fold_freq_hz * samps_per_hz);

   double cumul_val = 0.0;
   double denom = double(samps_per_hz)/2.0;

   for(int n=0; n<num_pts; n++)
   {
      freq = n / double(samps_per_hz);

      val = sinc_sqrd(freq);
      cumul_val += val;

      (*out_file) << freq << ", " << val << ", " << cumul_val/denom << endl;
   }
   out_file->close();
   delete out_file;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
