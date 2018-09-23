//
//  File = sinc_wave.cpp
//

#include <stdlib.h>
#include <math.h>
#include "sinc_wave.h"
#include "sinc.h"
#include <fstream>
using namespace std;
#ifndef PI
  #define PI 3.1415926535897932
#endif

void SincWaveform(  int num_sidelobes,
                    int samps_per_hump,
                    char* out_filename)
{
   double delta, val, t_val;
   ofstream *out_file;
   out_file = new ofstream(out_filename, ios::out);

   delta = 1.0/double(samps_per_hump);

   for(int n=-(num_sidelobes+1)*samps_per_hump; n<=(num_sidelobes+1)*samps_per_hump; n++)
   {
      t_val = n*delta;
      val = sinc(t_val);
      (*out_file) << t_val << ", " << val << endl;
   }
   delete out_file;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
