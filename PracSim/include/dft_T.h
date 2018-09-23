//
//  file = dft_T.h
//
#ifndef _DFT_T_H_
#define _DFT_T_H_

#include <stdlib.h> 
#include <math.h>
#include <complex>

void dft( std::complex<float> *x_in,
          std::complex<float> *y_out,
          int num_samps);
#endif
