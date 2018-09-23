//
//  File = poly_xf.h
//

#ifndef _POLY_XF_H_
#define _POLY_XF_H_
#include "xfelem.h"
#include "poly_int.h"
//#include "vector_pf.h"
//#include "rowvec_pf.h"

class PolyOvrExtenField
{
public:

  PolyOvrExtenField(void);
  PolyOvrExtenField(GaloisField *ext_field);
  PolyOvrExtenField(GaloisField *ext_field, int degree);
  PolyOvrExtenField(GaloisField *ext_field, int degree, ExtenFieldElem coeff);
  PolyOvrExtenField(GaloisField *ext_field, ExtenFieldElem coeff);
  //PolyOvrExtenField( int prime_base, PolyOvrIntegers *poly);
  //PolyOvrExtenField( int prime_base,
  //                   int degree,
  //                   rowvec_pf *coeff);
  //PolyOvrExtenField( int prime_base,
 //                    rowvec_pf *coeff);
  int MaxDegree(void);
  //int PrimeBase(void);
  int NumberOfTerms(void);
  int PenultimateDegree(void);
  ExtenFieldElem Coefficient(int degree);
  void GetCoeffs( ExtenFieldElem *coeff_vec);
  //void SetRemainder( PolyOvrPrimeField* poly );
  //void SetRemainder( int degree, PrimeFieldElem* coeff );

  //PolyOvrPrimeField& operator= (const PolyOvrPrimeField &right);
  void operator= (const PolyOvrExtenField &right);
  PolyOvrExtenField& operator* (const PolyOvrExtenField &right);
  PolyOvrExtenField& operator*= (const PolyOvrExtenField &right);
  PolyOvrExtenField& operator/ (const PolyOvrExtenField &divisor);
  PolyOvrExtenField& operator% (const PolyOvrExtenField &divisor);
  PolyOvrExtenField& operator/= (const PolyOvrExtenField &divisor);
  PolyOvrExtenField& operator%= (const PolyOvrExtenField &divisor);
  PolyOvrExtenField& operator-= (const ExtenFieldElem &const_term);
  void DumpToStream( ostream* output_stream);
  friend class MinimalPolynomial;
  friend PolyOvrExtenField* Derivative(const PolyOvrExtenField *right);
  friend PolyOvrExtenField& EuclideanAlgorithm( 
                                  const PolyOvrExtenField *poly1,
                                  const PolyOvrExtenField *poly2);


private:
  GaloisField* Exten_Field;
  int Degree;
  ExtenFieldElem* Coeff_Xf;
  //int Rem_Degree;
 // PrimeFieldElem* Rem_Coeff;
};


#endif
