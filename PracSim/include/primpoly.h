//
//  File = primpoly.h
//

#ifndef _PRIMPOLY_H_
#define _PRIMPOLY_H_
#include "pfelem.h"
#include "cycpoly.h"
#include "poly_pf.h"
#include "berlefac.h"
#include "deque"
using namespace std;

class PrimitivePolynomialSet
{
public:

  PrimitivePolynomialSet( int prime_field_base, int degree );
  PolyOvrPrimeField* GetSimplest(void);

private:
  int Deg_Prim_Polys;
  int Prime_Field_Base;
  int Max_Prim_Poly_Cnt;
  deque<PolyOvrPrimeField*> *Prim_Polys;
  CyclotomicPoly* Cyc_Poly;
};


#endif
