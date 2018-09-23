//
// File = dit_nipo_T.h
//
#ifndef _DIT_NIPO_T_H_
#define _DIT_NIPO_T_H_  

#include <complex>
//using namespace std;

template <class T>
void FftDitNipo( std::complex<T>* array, int fft_size); 

template <class T>
void IfftDitNipo( std::complex<T>* array, int fft_size); 
 
#endif // _DIT_NIPO_T_H_