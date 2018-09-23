//
//  File = gausrand.h
//
#ifndef _GAUSRAND_H_
#define _GAUSRAND_H_
#include <complex>

void GaussRandom(long* seed, double* rand_var);
void GaussRandom(long* seed, float* rand_var);
void GaussRandom(long* seed, std::complex<double> *rand_var);
void GaussRandom(long* seed, std::complex<float> *rand_var);

#endif      
