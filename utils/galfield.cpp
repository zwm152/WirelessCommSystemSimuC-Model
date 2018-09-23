// galfield.cpp
//

#include <iostream>
#include "galfield.h"
#include "isprime.h"
#include "xfelem.h"
#include <stdlib.h>

GaloisField::GaloisField( int base, 
                          int degree, 
                          PolyOvrPrimeField *prim_poly)
{
  Base = base;
  Degree = degree;
  Prim_Poly = prim_poly;
  Reduc_Poly = new PrimeFieldElem[degree+1];

  for(int term=(Prim_Poly->MaxDegree()); term>=0; term--)
    {
//    Reduc_Poly[term] = (base-(Prim_Poly->Coefficient(term)))%base;
    Reduc_Poly[term] = PrimeFieldElem(base,0)-(Prim_Poly->Coefficient(term));
    }

  #ifdef NOT_DEFINED
  if( ValueIsPrime(order) )
    {
    cout << "order is prime" << endl;
    //GenericField* pImplemField = new PrimeField(order);
    }
  else
    {
    cout << "order is composite" << endl;
    }
  #endif
}
int GaloisField::GetDegree(void)
{
  return Degree;
}
int GaloisField::GetBase(void)
{
  return Base;
}

ExtenFieldElem GaloisField::GetElement(int expon)
{
  ExtenFieldElem result(this);
  PrimeFieldElem *work;
  int dgt_idx,elm_idx;
  work = new PrimeFieldElem[Degree+1];
  //
  // Initialize working m-tuple to equal 0...01
  work[0]=PrimeFieldElem(Base,1);
  for(dgt_idx=1; dgt_idx<Degree; dgt_idx++)
    {
    work[dgt_idx]=PrimeFieldElem(Base,0);
    }
  // Build elements of field until the element alpha**expon
  // is reached
  for(elm_idx=1; elm_idx<=expon; elm_idx++)
    {
    // multiply working element by alpha
    for(dgt_idx=Degree; dgt_idx>=1; dgt_idx--)
      {
      work[dgt_idx]=work[dgt_idx-1];
      }
    work[0]=PrimeFieldElem(Base,0);
    // if multiplication causes an overflow,
    // reduce by subtracting alpha**m
    while(work[Degree]!=0)
      {
      for(dgt_idx=0; dgt_idx<=Degree; dgt_idx++)
        {
        work[dgt_idx] = Reduc_Poly[dgt_idx]+work[dgt_idx];
        }
      }
    //cout << endl;
    }
  // copy stuff into result object
  result.Field = this;
  for(dgt_idx=0; dgt_idx<Degree; dgt_idx++)
    {
    result.Value[dgt_idx] = work[dgt_idx];
    }
  return(result);
}