//
//  file = complex_io.h
//

#ifndef _COMPLEX_IO_H_
#define _COMPLEX_IO_H_ 

#include <fstream>
#include <complex>
using std::complex;
using namespace std;
//
fstream& operator<<(fstream&, const complex<float>&);
ostream& operator<<(ostream&, const complex<double>&);
istream& operator>>(istream&, complex<float>&);
istream& operator>>(istream&, complex<double>&);

#endif // _COMPLEX_IO_H_