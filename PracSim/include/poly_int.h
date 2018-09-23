//
//  File = poly_int.h
//

#ifndef _POLY_INT_H_
#define _POLY_INT_H_
#include <iostream>
using std::ostream;

class PolyOvrIntegers
{
public:

  PolyOvrIntegers(void);
  PolyOvrIntegers(int degree);
  PolyOvrIntegers(int degree, int coeff);
  int MaxDegree(void);
  int Coefficient(int degree);
  void SetRemainder( PolyOvrIntegers* poly );
  void SetRemainder( int degree, int* coeff );
  PolyOvrIntegers& operator* (const PolyOvrIntegers &right);
  PolyOvrIntegers& operator*= (const PolyOvrIntegers &right);
  PolyOvrIntegers& operator/ (const PolyOvrIntegers &divisor);
  PolyOvrIntegers& operator/= (const PolyOvrIntegers &divisor);
  void DumpToStream( ostream* output_stream);
  friend PolyOvrIntegers* Derivative(const PolyOvrIntegers *right);

private:
  int Degree;
  int* Coeff;
  int Rem_Degree;
  int* Rem_Coeff;
};


#endif
