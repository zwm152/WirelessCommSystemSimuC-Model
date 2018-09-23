//
//  File = galfield.h
//

#ifndef _GALFIELD_H_
#define _GALFIELD_H_

#include "poly_pf.h"
#include "xfelem.h"

class GaloisField
{
public:

  GaloisField( int base, int order, PolyOvrPrimeField *prim_poly );
  int GetDegree( void );
  int GetBase( void );
  ExtenFieldElem GetElement(int expon);
  friend class ExtFieldElem;
  friend ExtenFieldElem operator*( const ExtenFieldElem,
                               const ExtenFieldElem);

private:

  int Base;
  int Degree;
  PolyOvrPrimeField *Prim_Poly;
  PrimeFieldElem *Reduc_Poly;
  long dummy1;
};

#endif
