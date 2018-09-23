//
//  File = bilin_transf.h
//

#ifndef _BILIN_TRANSF_H_
#define _BILIN_TRANSF_H_
#include "denorm_proto.h"
using std::complex;

void BilinearTransf(
                     DenormalizedPrototype* analog_filter,
                     double samp_intvl,
                     double **a_coef,
                     double **b_coef); 

#endif                               
