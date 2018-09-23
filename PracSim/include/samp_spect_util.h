//
// File = samp_spect_util.h
//
#ifndef _SAMP_SPECT_UTIL_H_
#define _SAMP_SPECT_UTIL_H_  

#include <complex>
#include "spec_estim.h"


template <class T>
class SampleSpectrum : public SpectrumEstimator<T>
{
public:
   SampleSpectrum(int num_samps,
                  int fft_len,
                  double samp_intvl );

   void Calculate(  T* time_signal );
private:
   //int Num_Samps;
   //int Fft_Len;
   //double Samp_Intvl;
};


#endif // _SAMP_SPECT_H_