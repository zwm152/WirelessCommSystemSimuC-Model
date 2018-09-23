//
//  File = arspec.cpp
//

#include <fstream>
#include "ar_spec.h"
#include "misdefs.h"
using namespace std;

//==============================================
//  constructor

ArSpectrum::ArSpectrum( int ar_order,
                           double* ar_coeff,
                           double samp_intvl,
                           double drv_var,
                           int num_pts,
                           double max_freq)
{
  double denom, two_pi_f, psd_val;
  double a_func_real, a_func_imag;
  int f_idx, cof_idx;

  Samp_Intvl = samp_intvl;
  Num_Pts = num_pts;
  Spec_Buf = new double[Num_Pts];
  Max_Freq = max_freq;

  Freq_Delt = Max_Freq*Samp_Intvl/double(Num_Pts-1);
  double total_pwr = 0.0;
  for(f_idx=0; f_idx<Num_Pts; f_idx++)
    {
    a_func_real = 0.0;
    a_func_imag = 0.0;
    two_pi_f = TWO_PI * f_idx * Freq_Delt;
    for(cof_idx=0; cof_idx<=ar_order; cof_idx++)
      {
      a_func_real += (ar_coeff[cof_idx]*cos(cof_idx*two_pi_f));
      a_func_imag -= (ar_coeff[cof_idx]*sin(cof_idx*two_pi_f));
      }
    denom = a_func_real*a_func_real + a_func_imag*a_func_imag;
    psd_val = samp_intvl*drv_var/denom;
    total_pwr += psd_val;
    Spec_Buf[f_idx] = psd_val;
    }
  return;
}
//------------------------
// destructor

ArSpectrum::~ArSpectrum(void){};

void ArSpectrum::DumpSpectrum( char* out_file_nam,
                                  bool db_plot_enab )
{
  int i;
  double freq, vert_offset;
  ofstream out_file(out_file_nam, ios::out);

  vert_offset = 10.0 * log10(Spec_Buf[0]);
  for(i=0; i<Num_Pts; i++)
    {
    freq = i*Freq_Delt/Samp_Intvl;
    if( db_plot_enab) {
      out_file << freq << ", " 
               << ((10.0 * log10(Spec_Buf[i]))-vert_offset) 
               << endl;
      }
    else {
      out_file << freq << ", " << (Spec_Buf[i])
               << endl;
      }
    }
  out_file.close();
}


