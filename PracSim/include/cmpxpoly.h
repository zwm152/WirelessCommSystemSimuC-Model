//
// File = cmpxpoly.h
//
#ifndef _CMPXPOLY_H_
#define _CMPXPOLY_H_  

#include <fstream>
#include <complex>
using namespace std;

class CmplxPolynomial
{
public: 

  //  default constructor
  CmplxPolynomial( );
  
  //  copy constructor
  CmplxPolynomial( const CmplxPolynomial &original);
  
  // constructor for initializing a binomial
  CmplxPolynomial( const complex<double> coeff_1,
                   const complex<double> coeff_0);
                   
  //  initializing constructor
  CmplxPolynomial( const int degree,
                   const complex<double> *coeff);
                   
  //  initializing constructor
  CmplxPolynomial( const double *coeff,
                   const int degree);
                   
  // assignment operator
  CmplxPolynomial& operator= (const CmplxPolynomial &right);
  
  //  multiply assign operator
  CmplxPolynomial& operator*= (const CmplxPolynomial &right);
  
  //  divide assign operator
  CmplxPolynomial& operator/= (const CmplxPolynomial &divisor);
  
  // return array of polynomial root values
  complex<double>* GetRoots( void );

  // reflect root across the unit circle
  void ReflectRoot( int root_idx );

  // dump polynomial to an output stream
  void DumpToStream( ostream* output_stream);
  
  // get degree of polynomial
  int GetDegree(void); 
  
  // return specified coefficient
  complex<double> GetCoeff(int k);

  // return pointer to copy of coefficients
  void CopyCoeffs(complex<double>*);

  friend class RealPolynomial;

private:

  // find roots of the polynomial
  void FindRoots( void );

  // build sum of powers coefficients from roots
  //void BuildFromRoots( void );
   
  int Poly_Degree;
  complex<double>* Poly_Coeff; 
  complex<double>* RemCoeff;
  complex<double>* Root;
}; 
#endif 