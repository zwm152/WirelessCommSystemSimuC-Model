//
//  file = complex_io.cpp
//

#include <ostream>
#include <iostream>

#include "complex_io.h"  
using namespace std;

ostream& operator<<( ostream& s, const std::complex<float>& z)
{
 //s << "(" << z.real() << "," << z.imag() << ")";
 s << z.real() << "," << z.imag();
 return s;
}

ostream& operator<<( ostream& s, const std::complex<double>& z)
{
 //s << "(" << z.real() << "," << z.imag() << ")";
 s << z.real() << "," << z.imag();
 return s;
}

istream& operator>>( istream& s, std::complex<float>& z)
{
  char lpar, rpar, comma;
  float _r, _i;
  for(;;)
    {
    if( (!(s >> lpar >> _r >> comma >> _i >> rpar))
        || (lpar != '(') || (rpar != ')') 
        || (comma != ',') ) 
      {
      // bad stream input
      s.clear();
      cout << "format for complex value is '( double, double)'\n"
           << " -- try again" << endl;
      continue;
      }
    break;
    }
  z = std::complex<float>(_r, _i);
  return s;
}
istream& operator>>( istream& s, std::complex<double>& z)
{
  char lpar, rpar, comma;
  double _r, _i;
  for(;;)
    {
    if( (!(s >> lpar >> _r >> comma >> _i >> rpar))
        || (lpar != '(') || (rpar != ')') 
        || (comma != ',') ) 
      {
      // bad stream input
      s.clear();
      cout << "format for complex value is '( double, double)'\n"
           << " -- try again" << endl;
      continue;
      }
    break;
    }
  z = std::complex<double>(_r, _i);
  return s;
}
