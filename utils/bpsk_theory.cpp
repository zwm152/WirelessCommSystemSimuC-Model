//
//  File = bpsk_theory.cpp
//

#include <stdlib.h>
#include <math.h>
#include "bpsk_theory.h"
#include "sinc.h"
#include "q_func.h"
#include <fstream>
using namespace std;
#ifndef PI
  #define PI 3.1415926535897932
#endif

void BpskPsd( double carrier_freq,
              double symb_energy,
              double symb_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename)
{
  double freq, val, bit_energy;
  ofstream *out_file;
  out_file = new ofstream(out_filename, ios::out);

  bit_energy = symb_energy/2.0;

  double delta_freq = (end_freq - beg_freq)/double(num_pts-1);
  for(int n=0; n<num_pts; n++)
    {
    freq = beg_freq + n * delta_freq;
    val = bit_energy * (  sinc_sqrd((freq-carrier_freq)/symb_rate)
                        + sinc_sqrd((freq+carrier_freq)/symb_rate) );
    if( plot_in_db )
      {
      if(val>0.0)
        {
        val = 10.0 * log10(val);
        }
      else
        {
        val = -200.0;
        }
      }
    (*out_file) << freq << ", " << val << endl;
    }
  delete out_file;
}

//==========================================================

void BpskBer( double beg_ebno,
              double end_ebno,
              int num_pts,
              char* out_filename)
{
  double ebno_numeric, ebno_db, bit_err, symb_err, q_arg;
  ofstream *out_file;
  out_file = new ofstream(out_filename, ios::out);

  double delta_ebno = (end_ebno - beg_ebno)/double(num_pts-1);
  for(int n=0; n<num_pts; n++)
    {
    ebno_db = beg_ebno + n * delta_ebno;
    ebno_numeric = pow(10.0, ebno_db/10.0);
    q_arg = sqrt(2.0*ebno_numeric);
    bit_err = q_func(q_arg);
    //bit_err = 0.5 * erfc(sqrt(ebno_numeric));
    symb_err = bit_err * (2.0 - bit_err);
    (*out_file) << ebno_db << ", " << bit_err << ", " << symb_err << endl;
    }
  delete out_file;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
