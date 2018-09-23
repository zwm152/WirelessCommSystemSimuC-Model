//
// File = fft_T.h
//
#ifndef _FFT_T_H_
#define _FFT_T_H_  

//#include "std_cmpx.h"
#include <complex>
//using std::complex;

template< class T>
void FFT( std::complex<T>* input, std::complex<T>* output, int size );  

template <class T>
void FFT( float* time_signal,
          std::complex<T>* sample_spectrum,
          int num_samps,
          int fft_len );

template <class T>
void FFT( double* time_signal,
          std::complex<T>* sample_spectrum,
          int num_samps,
          int fft_len );

//template <class T>
//void FFT( T* time_signal,
//          std::complex<T>* sample_spectrum,
//          int num_samps,
//          int fft_len );

template< class T>
void IFFT( std::complex<T>* input, std::complex<T>* output, int size );  

template< class T>
void FFT( std::complex<float>* input, std::complex<T>* output, 
          int num_samps, int fft_len );  

template< class T>
void FFT( std::complex<double>* input, std::complex<T>* output, 
          int num_samps, int fft_len );  

template< class T>
void FFT( std::complex<T>* signal, int size );  

#endif // _FFT_T_H_