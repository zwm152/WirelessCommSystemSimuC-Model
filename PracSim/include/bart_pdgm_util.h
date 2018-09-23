//
// File = bart_pdgm_util;.h
//
#ifndef _BART_PDGM_UTIL_H_
#define _BART_PDGM_UTIL_H_  

#include <complex>
#include "spec_estim.h"
using namespace std;


template <class T>
class BartlettPeriodogram : public SpectrumEstimator<T>
{
public:
   BartlettPeriodogram(int num_samps,
                  int fft_len,
                  double samp_intvl );

   void Calculate(  T* time_signal );
private:
   complex<double>* Freq_Signal;
   //int Num_Samps;
   //int Fft_Len;
   //double Samp_Intvl;
};


#endif // _BART_PDGM_H_