//
//  File = poly_pf.h
//

#ifndef _POLY_PF_H_
#define _POLY_PF_H_
#include "pfelem.h"
#include "poly_int.h"
#include "vector_pf.h"
#include "rowvec_pf.h"

class PolyOvrPrimeField
{
public:

  PolyOvrPrimeField(void);
  PolyOvrPrimeField(int prime_base);
  PolyOvrPrimeField(int prime_base, int degree);
  PolyOvrPrimeField(int prime_base, int degree, PrimeFieldElem coeff);
  PolyOvrPrimeField( int prime_base, PolyOvrIntegers *poly);
  PolyOvrPrimeField( int prime_base,
                     int degree,
                     rowvec_pf *coeff);
  PolyOvrPrimeField( int prime_base,
                     rowvec_pf *coeff);
  PolyOvrPrimeField( int prime_base,
                     int degree, 
                     PrimeFieldElem* coeff[]);
  int MaxDegree(void);
  int PrimeBase(void);
  int NumberOfTerms(void);
  int PenultimateDegree(void);
  PrimeFieldElem Coefficient(int degree);
  void GetCoeffs( PrimeFieldElem *coeff_vec);
  //void SetRemainder( PolyOvrPrimeField* poly );
  //void SetRemainder( int degree, PrimeFieldElem* coeff );

  //PolyOvrPrimeField& operator= (const PolyOvrPrimeField &right);
  void operator= (const PolyOvrPrimeField &right);
  PolyOvrPrimeField& operator* (const PolyOvrPrimeField &right);
  PolyOvrPrimeField& operator*= (const PolyOvrPrimeField &right);
  PolyOvrPrimeField& operator/ (const PolyOvrPrimeField &divisor);
  PolyOvrPrimeField& operator% (const PolyOvrPrimeField &divisor);
  PolyOvrPrimeField& operator/= (const PolyOvrPrimeField &divisor);
  PolyOvrPrimeField& operator%= (const PolyOvrPrimeField &divisor);
  PolyOvrPrimeField& operator-= (const PrimeFieldElem &const_term);
  void DumpToStream( ostream* output_stream);
  friend PolyOvrPrimeField* Derivative(const PolyOvrPrimeField *right);
  friend PolyOvrPrimeField& EuclideanAlgorithm( 
                                  const PolyOvrPrimeField *poly1,
                                  const PolyOvrPrimeField *poly2);


protected:
  int Prime_Base;
  int Degree;
  PrimeFieldElem* Coeff;
  //int Rem_Degree;
 // PrimeFieldElem* Rem_Coeff;
};


#endif
