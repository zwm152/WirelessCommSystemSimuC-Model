//
// File = realpoly.h
//
#ifndef _REALPOLY_H_
#define _REALPOLY_H_  

#include <fstream>
#include "cmpxpoly.h"


class RealPolynomial
{
public: 

  //  default constructor
  RealPolynomial( );
  
  // constructor for initializing a constant
  RealPolynomial( const double coeff_0);
                   
  //  copy constructor
  RealPolynomial( const RealPolynomial &original);
  
  //  conversion constructor
  RealPolynomial( const CmplxPolynomial &original);

  // constructor for initializing a binomial
  RealPolynomial( const double coeff_1,
              const double coeff_0);
                   
  // constructor for initializing a quadratic
  RealPolynomial( const double coeff_2,
              const double coeff_1,
              const double coeff_0);
                   
  // initializing constructor
  RealPolynomial( const int degree, const double* const coefficient);

  // assignment operator
  RealPolynomial& operator= (const RealPolynomial &right);
  
  //  multiply assign operator
  RealPolynomial& operator*= (const RealPolynomial &right);

  //  add assign operator
  RealPolynomial& operator+= (const RealPolynomial &right);

  // multiply by another polynomial
  RealPolynomial operator* (const RealPolynomial &right);

  // add another polynomial
  RealPolynomial operator+ (const RealPolynomial &right);

  // multiply by double and assign operator        
  RealPolynomial& RealPolynomial::operator*= (double right);
  
  //  divide assign operator
  RealPolynomial& operator/= (const RealPolynomial &right);
  
  // dump polynomial to an output stream
  void DumpToStream( ofstream* output_stream);

  // get degree of polynomial
  int RealPolynomial::GetDegree(void); 
  
  // return specified coefficient
  double RealPolynomial::GetCoefficient(int k);

  friend RealPolynomial operator* (const RealPolynomial &poly, const double d_val);
  friend RealPolynomial operator* (const double d_val, const RealPolynomial &poly);

private:
   
  int Poly_Degree;
  double* Poly_Coeff; 
}; 
#endif 