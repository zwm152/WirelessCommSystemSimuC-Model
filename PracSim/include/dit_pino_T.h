//
// File = dit_pino_T.h
//
#ifndef _DIT_PINO_T_H_
#define _DIT_PINO_T_H_  

#include <complex>
//using namespace std;

template< class T>
void FftDitPino( std::complex<T>* array, int fft_size); 

template <class T>
void IfftDitPino( std::complex<T>* array, int fft_size); 
 
#endif // _DIT_PINO_T_H_