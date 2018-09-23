//
//  File = cycpoly.h
//

#ifndef _CYCPOLY_H_
#define _CYCPOLY_H_

#include "galfield.h"
//#include "polynom.h"
#include "poly_int.h"

int Moebius(int m);

class CyclotomicPoly : public PolyOvrIntegers
{
public:

  //CyclotomicPoly( GaloisField *gf );
  CyclotomicPoly( int e_val, int prime_char );

private:
  //GaloisField* pGalFld;

};

#endif
