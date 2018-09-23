//
//  File = berlemat.h
//

#ifndef _BERLEMAT_H_
#define _BERLEMAT_H_
#include "pfelem.h"
#include "matrix_pf.h"
#include "poly_pf.h"

class BerlekampMatrix:public matrix_pf
{
public:
  BerlekampMatrix(  PolyOvrPrimeField *poly );
  int GetDegree( void );
  int PrimeBase( void );
private:
  int Degree;
  int Prime_Base;
};
#endif
