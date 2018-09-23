//
// File = laguerre.h
//
#ifndef _LAGUERRE_H_
#define _LAGUERRE_H_  

#include "realpoly.h"

int LaguerreMethod( CmplxPolynomial *poly,
                    std::complex<double> *root_ptr,
                    double epsilon,
                    double epsilon2,
                    int max_iter);

#endif 