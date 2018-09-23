// poly_pf.cpp
//

#include <iostream>
#include <fstream>
#include "poly_pf.h"
#include "pfelem.h"
#include "stdlib.h"
extern ofstream DebugFile;

//============================================================
PolyOvrPrimeField::PolyOvrPrimeField( int prime_base,
                                      int degree,
                                      rowvec_pf *coeff)
{
  int i;
  Prime_Base = prime_base;
  Degree = degree;
  Coeff = new PrimeFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
    {
    Coeff[i] = ((*coeff)[i]);
    }
}
//============================================================
PolyOvrPrimeField::PolyOvrPrimeField( int prime_base,
                                      rowvec_pf *coeff)
{
  int i;
  int len = coeff->getlen();
  Prime_Base = prime_base;
  Degree = 0;
  for(i=0; i<len; i++)
    {
    if( (*coeff)[i].Value != 0 ) Degree=i;
    }
  Coeff = new PrimeFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
    {
    Coeff[i] = (*coeff)[i];
    }
}