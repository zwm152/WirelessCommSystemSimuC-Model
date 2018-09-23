//
//  File = ar_est.h
//

#ifndef _ARSPEC_H_
#define _ARSPEC_H_

#include "typedefs.h"

class ArSpectrum
{
public:
  ArSpectrum( int ar_order,
              double* ar_coeff,
              double samp_invl,
              double drv_var,
              int num_pts,
              double max_freq);

  ~ArSpectrum(void);
  void DumpSpectrum( char* out_file_name,
                     bool db_plot_enab);
private:
  int Ar_Order;
  int Num_Pts;
  double Max_Freq;
  double Freq_Delt;
  double *Spec_Buf;
  double Samp_Intvl;
};

#endif
