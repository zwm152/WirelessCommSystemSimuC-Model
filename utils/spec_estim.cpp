//
//  File = samp.cpp
//

#include <iostream> 
#include <fstream>

#include <complex>
#include "fft_T.h"
#include "spec_estim.h"
using std::complex;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
template <class T>
SpectrumEstimator<T>::SpectrumEstimator(  int num_samps,
                                          int fft_len,
                                          double samp_intvl )
{
   Num_Samps = num_samps;
   Fft_Len = fft_len;
   Samp_Intvl = samp_intvl;
   Psd_Estimate = new double[Num_Samps];
   for(int i=0; i<Num_Samps; i++) Psd_Estimate[i]=0.0;
}
//=================================================================
template <class T>
void SpectrumEstimator<T>::GetEstimate(  double* psd_estimate )
{
   int i;
   for(i=0; i<Num_Samps; i++)
   {
      psd_estimate[i] = Psd_Estimate[i];
   }
   return;
}
template SpectrumEstimator<std::complex<float> >;
template SpectrumEstimator<float>;

