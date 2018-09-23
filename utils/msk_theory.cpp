//
//  File = msk_theory.cpp
//

#include <stdlib.h>
#include <math.h>
#include "msk_theory.h"
#include "sinc.h"
#include "q_func.h"
#include <fstream>
#ifndef PI
  #define PI 3.1415926535897932
#endif
using namespace std;

void MskPsd( double carrier_freq,
              double bit_energy,
              double bit_intvl,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename)
{
  double freq, val;
  double work, numer, denom;
  double first_term, second_term;
  double scale;
  ofstream *out_file;
  out_file = new ofstream(out_filename, ios::out);

  scale = PI * PI / 8.0;
  double delta_freq = (end_freq - beg_freq)/double(num_pts-1);
  for(int n=0; n<num_pts; n++)
    {
    freq = beg_freq + n * delta_freq;

    // calculate first term

    work = 2.0 * bit_intvl * ( freq - carrier_freq );
    denom = 1.0 - 4.0*work*work;
    if(denom == 0.0)
      {
      //first_term = PI*PI/16.0;
      first_term = 0.25;
      }
    else
      {
      numer = 2.0*cos(PI * work);
      denom *= PI;
      first_term = numer/denom;
      first_term *= first_term;
      }
    //------------------------------------------
    // calculate second term

    work = 2.0 * bit_intvl * ( freq + carrier_freq );
    denom = 1.0 - 4.0*work*work;
    if(denom == 0.0)
      {
      //second_term = PI*PI/16.0;
      second_term = 0.25;
      }
    else
      {
      numer = 2.0*cos(PI * work);
      denom *= PI;
      second_term = numer/denom;
      second_term *= second_term;
      }
    val = scale * bit_energy * (first_term + second_term);
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

void MskBer( double beg_ebno,
              double end_ebno,
              int num_pts,
              char* out_filename)
{
  double ebno_numeric, ebno_db, bit_err, q_arg;
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
    (*out_file) << ebno_db << ", " << bit_err << endl;
    }
  delete out_file;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
