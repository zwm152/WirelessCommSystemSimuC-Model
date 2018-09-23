//
// File = poly.h
//
#ifndef _POLY_H_
#define _POLY_H_  

#include <fstream.h>
#include "cmpxpoly.h"


class Polynomial
{
public: 

  //  default constructor
  Polynomial( );
  
  //  copy constructor
  Polynomial( const Polynomial &original);
  
  //  conversion constructor
  Polynomial( const CmplxPolynomial<double> &original);
  
  // constructor for initializing a monomial
  Polynomial( const double coeff_0);
                   
  // constructor for initializing a binomial
  Polynomial( const double coeff_1,
              const double coeff_0);
                   
  // constructor for initializing a quadratic
  Polynomial( const double coeff_2,
              const double coeff_1,
              const double coeff_0);
                   
  // assignment operator
  Polynomial& operator= (const Polynomial &right);
  
  //  multiply assign operator
  Polynomial& operator*= (const Polynomial &right);
  
  //  divide assign operator
  Polynomial& operator/= (const Polynomial &right);
  
  // dump polynomial to an output stream
  void DumpToStream( ofstream* output_stream);

  // get degree of polynomial
  int Polynomial::GetDegree(void); 
  
  // return specified coefficient
  double Polynomial::GetCoefficient(int k);

private:
   
  int Degree;
  double* Coefficient; 
}; 
#endif 