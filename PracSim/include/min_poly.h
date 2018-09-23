//
//  File = min_poly.h
//

#ifndef _MIN_POLY_H_
#define _MIN_POLY_H_
#include "galfield.h"
#include "cyclo_partit.h"
//#include "pfelem.h"
//#include "cycpoly.h"
#include "poly_pf.h"
//#include "berlefac.h"
//#include "deque"
using namespace std;

class MinimalPolynomial : public PolyOvrPrimeField
{
public:

  MinimalPolynomial( GaloisField* exten_field, CyclotomicCoset *coset );

private:
  int* Coset_Start;
  int* Coset_Size;
  int* Element_Vector;
  int Num_Cosets;
  int Modulus;
};


#endif
