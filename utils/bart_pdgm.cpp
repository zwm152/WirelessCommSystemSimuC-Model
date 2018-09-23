//
//  File = bart_pdgm.cpp
//

#include <iostream> 
#include <fstream>

#include <complex>
#include "fft_T.h"
#include "bart_pdgm_util.h"
using std::complex;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
template <class T>
BartlettPeriodogram<T>::BartlettPeriodogram(  int num_samps,
                                    int fft_len,
                                    double samp_intvl )
                  :SpectrumEstimator<T>(  num_samps,
                                       fft_len,
                                       samp_intvl)
{
   Freq_Signal = new complex<double>[Fft_Len];
};
//=================================================================
template <class T>
void BartlettPeriodogram<T>::Calculate(  T* time_signal )
{
   int i;


   FFT<double>( time_signal,
                 Freq_Signal,
                 Num_Samps,
                 Fft_Len );
   for(i=0; i<Num_Samps; i++)
   {
      Psd_Estimate[i] += Samp_Intvl * std::norm(Freq_Signal[i])/Num_Samps;
      //Psd_Estimate[i] += Samp_Intvl * std::norm(freq_signal[i])/(Num_Samps*Num_Samps);
   }
   return;
}
template BartlettPeriodogram< std::complex<float> >;
template BartlettPeriodogram<float>;

