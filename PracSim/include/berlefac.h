//
//  File = berlefac.h
//

#ifndef _BERLEFAC_H_
#define _BERLEFAC_H_
#include "pfelem.h"
#include "poly_pf.h"
#include "berlemat.h"

class BerlekampFactorization
{
public:

  BerlekampFactorization( PolyOvrPrimeField *poly );
  PolyOvrPrimeField* GetFactor( int factor_index );
  int GetR(void);

private:
  int Degree;
  BerlekampMatrix *B_Mtx;
  PolyOvrPrimeField **Factors;
  int R;
};


#endif
