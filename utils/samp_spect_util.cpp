//
//  File = samp.cpp
//

#include <iostream> 
#include <fstream>

#include <complex>
#include "fft_T.h"
#include "samp_spect_util.h"
using std::complex;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
template <class T>
SampleSpectrum<T>::SampleSpectrum(  int num_samps,
                                    int fft_len,
                                    double samp_intvl )
                  :SpectrumEstimator<T>(  num_samps,
                                       fft_len,
                                       samp_intvl)
{
}
//=================================================================
template <class T>
void SampleSpectrum<T>::Calculate(  T* time_signal )
{
   int i;
   complex<double>* freq_signal;

   freq_signal = new complex<double>[Fft_Len];

   FFT<double>( time_signal,
                 freq_signal,
                 Num_Samps,
                 Fft_Len );
   for(i=0; i<Num_Samps; i++)
   {
      Psd_Estimate[i] = Samp_Intvl * std::norm(freq_signal[i])/Num_Samps;
   }
   return;
}
template SampleSpectrum<float>;
template SampleSpectrum< std::complex<float> >;

