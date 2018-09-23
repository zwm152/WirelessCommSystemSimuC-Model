//
//  File = nrz_theory.cpp
//

#include <stdlib.h>
#include <math.h>
#include "nrz_theory.h"
#include "sinc.h"
#include <fstream>
#ifndef PI
  #define PI 3.1415926535897932
#endif
using namespace std;

void NrzPsd(  double bit_energy,
              double bit_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename)
{
  double freq, val;
  ofstream *out_file;
  out_file = new ofstream(out_filename, ios::out);

  double delta_freq = (end_freq - beg_freq)/double(num_pts-1);
  for(int n=0; n<num_pts; n++)
    {
    freq = beg_freq + n * delta_freq;
    val = bit_energy * sinc_sqrd(freq/bit_rate);
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
//================================================================
void BiphasePsd(  double bit_energy,
              double bit_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename)
{
  double freq, val, argument;
  ofstream *out_file;
  out_file = new ofstream(out_filename, ios::out);

  double delta_freq = (end_freq - beg_freq)/double(num_pts-1);
  for(int n=0; n<num_pts; n++)
    {
    freq = beg_freq + n * delta_freq;
    argument = freq/bit_rate/2.0;
    val = sin(PI*argument);
    val *= val;
    val *= bit_energy * sinc_sqrd(argument);
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

//================================================================
void MillerPsd(  double bit_energy,
              double bit_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename)
{
  double freq, val, phi;
  ofstream *out_file;
  out_file = new ofstream(out_filename, ios::out);

  double delta_freq = (end_freq - beg_freq)/double(num_pts-1);
  for(int n=0; n<num_pts; n++)
    {
    freq = beg_freq + n * delta_freq;
    phi = PI*freq/bit_rate;
    val = 23 - 2.0*cos(phi) - 22.0*cos(2.0*phi) - 12.0*cos(3.0*phi) + 5.0*cos(4.0*phi);
    val += 12.0*cos(5.0*phi) + 2.0*cos(6.0*phi) - 8.0*cos(7.0*phi) + 2.0*cos(8.0*phi);
    val /= phi * phi * (34.0 + 16.0*cos(8.0*phi));
    val *= bit_energy;
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

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
