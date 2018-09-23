//
// berlemat.cpp
//

#include <iostream>
#include "berlemat.h"
#include <fstream>
extern ofstream DebugFile;

BerlekampMatrix::BerlekampMatrix( PolyOvrPrimeField *poly )
{
  int i, k1, k2;
  int deg = poly->MaxDegree();
  Degree = deg;
  int prime_char = poly->PrimeBase();
  Prime_Base = prime_char;
  allocate(0,deg,0,deg);
  //-----------------------------------------------------
  //  Populate B matrix

  PrimeFieldElem *work_vec = new PrimeFieldElem[deg+1];
  PrimeFieldElem *u;
  PrimeFieldElem temp;
  u = new PrimeFieldElem[deg+1];
  poly->GetCoeffs(u);

  for(i=1; i<=deg; i++) work_vec[i] = PrimeFieldElem(prime_char,0);
  work_vec[0] = PrimeFieldElem(prime_char,1);

  setrow(0,work_vec);

  for( k1=1; k1<deg; k1++)
    {
    for( k2=1; k2<=prime_char; k2++)
      {
      for( i=deg; i>0; i--)
        work_vec[i] = work_vec[i-1];
      work_vec[0] = PrimeFieldElem(prime_char,0);

      if( work_vec[deg] != 0 )
        {
        // reduce work_vec modulo u(x) using arithmetic
        // over GF(p) on the coefficients
        for( i=0; i<deg; i++)
          {
          temp = work_vec[deg] * u[i];
          work_vec[i] -= work_vec[deg] * u[i];
          }
        } // end of 'if( work_vec[deg] != 0 )'
      } // end of loop over k2
//    B_Mtx->setrow(k1,work_vec);
    setrow(k1,work_vec);
    } // end of loop over k1

  //-----------------------------------------------------
  //  Subtract identity matrix from B
  for(i=0; i<deg; i++)
    {
    (operator[](i)).operator[](i) -= PrimeFieldElem(prime_char,1);
    }
};
int BerlekampMatrix::GetDegree( void )
{
return(Degree);
};
int BerlekampMatrix::PrimeBase( void )
{
return(Prime_Base);
};
