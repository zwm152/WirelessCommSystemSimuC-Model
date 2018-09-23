//
// berlefac.cpp
//

#include <iostream>
#include "berlefac.h"
#include "nullspac.h"
#include <fstream>
extern ofstream DebugFile;

//=====================================================
BerlekampFactorization::BerlekampFactorization( 
                                  PolyOvrPrimeField *orig_poly )
{

  int k, r, s;
  int poly_idx;
  int deg = orig_poly->MaxDegree();
  int prime_char = orig_poly->PrimeBase();
  matrix_pf *v;
  PolyOvrPrimeField **polys;

  int num_facs = 1;
  int num_polys;

  Factors = new PolyOvrPrimeField*[1];
  Factors[0] = orig_poly;

  for(int iter=0; iter<1; iter++)
    {
    num_polys = num_facs;
    polys = new PolyOvrPrimeField*[num_polys];

    for( poly_idx=0; poly_idx<num_polys; poly_idx++)
      {
      polys[poly_idx] = Factors[poly_idx];
    int yydeg = (polys[poly_idx])->MaxDegree();
     // delete (Factors[poly_idx]);
      } // end of loop over poly_idx
    delete[] Factors;
    num_facs = 0;
    Factors = new PolyOvrPrimeField*[20];

    for( poly_idx=0; poly_idx<num_polys; poly_idx++)
      {
//int xxdeg = (polys[poly_idx])->MaxDegree();
//if(xxdeg == 7) continue;
      B_Mtx = new BerlekampMatrix( polys[poly_idx] );
      //  Row k corresponds to the term x**(k*p) where p is the
      //  characteristic of the prime field over which the polynomial
      //  is to be factored.

      //-----------------------------------------------------
      //  Compute null space of B matrix

      v = NullSpace(B_Mtx, &r);

      //----------------------------------------------------
      // Find factors
  
      //DebugFile << "rrr = " << r << endl;
      R = r;
      //
      //  The original polynomial should factor into r irreducible
      //  polynomials.  However each iteration of the Berlekamp
      //  factorization will only factor the original polynomial
      //  into p polynomials (which, in general, may be reducible)
      //  where p is the prime characteristic of the field from 
      //  which the coefficients are drawn.  Additional iterations
      //  can be performed until all r irreducible factors are found.

     // Factors = new PolyOvrPrimeField*[r];

      //PolyOvrPrimeField factor_poly;
      //Factors[0] = new PolyOvrPrimeField;
      for(k=1; k<2; k++)
        {
        //--------------------------------------------------------------
        //  Use the row 1 elements of v as coefficients for a polynomial

        PolyOvrPrimeField *work_poly = new PolyOvrPrimeField( prime_char,
                                                              &((*v)[k]));
        //DebugFile << "work_poly:" << endl;
        //work_poly->DumpToStream(&DebugFile);
        //DebugFile << "poly:" << endl;
        //polys[poly_idx]->DumpToStream(&DebugFile);

        for(s=0; s<prime_char; s++)
          {
          Factors[num_facs] = &(EuclideanAlgorithm( polys[poly_idx], work_poly ) );
          //DebugFile << "factor_poly (for k=" << k <<", s=" << s << "):" << endl;
          //(Factors[num_facs])->DumpToStream(&DebugFile);
          (*work_poly) -= PrimeFieldElem( prime_char, 1 );
          num_facs++;
          }
        delete work_poly;
        delete B_Mtx;
        delete v;

        } // end of loop over k
      } // end of loop over poly_idx
  } // end of loop over iter
}
int BerlekampFactorization::GetR(void)
{
  return(R);
}
PolyOvrPrimeField* BerlekampFactorization::GetFactor(int fac_idx)
{
  return(Factors[fac_idx]);
}
