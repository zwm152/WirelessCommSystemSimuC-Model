//
// File = spec_estim.h
//
#ifndef _SPEC_ESTIM_H_
#define _SPEC_ESTIM_H_  

#include <complex>


template <class T>
class SpectrumEstimator
{
public:
   SpectrumEstimator(int num_samps,
                  int fft_len,
                  double samp_intvl );

   virtual void Calculate(  T* time_signal ){};
   void GetEstimate(double *psd_est);
protected:
   int Num_Samps;
   int Fft_Len;
   double Samp_Intvl;
   double *Psd_Estimate;
};


#endif // _SPEC_ESTIM_H_