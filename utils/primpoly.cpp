//
// primpoly.cpp
//

#include <iostream>
#include <fstream>
#include "primpoly.h"
//using namespace std;
extern ::ofstream DebugFile;

int iipow(int x, int m)
{
 int i;
 int result;
 result = 1;
 for(i=1; i<=m; i++)
   {
    result *= x;
   }
 return(result);
}

//=====================================================
PrimitivePolynomialSet::PrimitivePolynomialSet( int prime_field_base,
                                                int degree )
{
  PolyOvrPrimeField *pf_poly;
  deque<PolyOvrPrimeField*> reduc_polys;
  int prim_poly_cnt, iter;
  BerlekampFactorization *berle_fac;
  Deg_Prim_Polys = degree;
  Prime_Field_Base = prime_field_base;
  Prim_Polys = new   deque<PolyOvrPrimeField*>;


  int cyc_poly_numb = iipow(prime_field_base, degree)-1;

  Cyc_Poly = new CyclotomicPoly(  cyc_poly_numb, prime_field_base);
  //DebugFile << "Cyclotomic polynomial (int):" << endl;
  //Cyc_Poly->DumpToStream(&DebugFile);
  pf_poly = new PolyOvrPrimeField(  prime_field_base, 
                                    Cyc_Poly);

  reduc_polys.push_back( pf_poly );
  prim_poly_cnt = 0;
  Max_Prim_Poly_Cnt = 9999;
  iter=0;

  while( prim_poly_cnt < Max_Prim_Poly_Cnt)
    {
    berle_fac = new BerlekampFactorization( reduc_polys.front() );
    reduc_polys.pop_front();
    if(iter==0) Max_Prim_Poly_Cnt = berle_fac->GetR();

    for( int fac_idx=0; fac_idx<prime_field_base; fac_idx++)
      {
      pf_poly = berle_fac->GetFactor(fac_idx);
      if( pf_poly->MaxDegree() == Deg_Prim_Polys )
        {
        // add polynomial to list of primitive polynomials
        Prim_Polys->push_back( pf_poly );
        prim_poly_cnt++;
        }
      else
        {
        //  add polynomial to list of reducible, nonprimitive 
        //  polynomials that will be factored further in 
        //  subsequent iterations of the while loop until 
        //  all mMaxPrimPolyCnt primitive polynomials are found

        reduc_polys.push_back( pf_poly );
        }
      }//end of loop over iFac
    iter++;
    } // end of while loop
}
//=======================================================
PolyOvrPrimeField* PrimitivePolynomialSet::GetSimplest(void)
{
  int min_num_terms = Deg_Prim_Polys+1;
  int deg_2nd_term = Deg_Prim_Polys;
  int simplest_idx;

  for(int i=0; i<Max_Prim_Poly_Cnt; i++)
    {
    if( (Prim_Polys->at(i))->NumberOfTerms() == min_num_terms)
      {
      if( (Prim_Polys->at(i))->PenultimateDegree() < deg_2nd_term )
        {
        deg_2nd_term = (Prim_Polys->at(i))->PenultimateDegree();
        simplest_idx = i;
        }
      }
    if( (Prim_Polys->at(i))->NumberOfTerms() < min_num_terms)
      {
      min_num_terms = (Prim_Polys->at(i))->NumberOfTerms();
      deg_2nd_term = (Prim_Polys->at(i))->PenultimateDegree();
      simplest_idx = i;
      }
    }
  return( Prim_Polys->at(simplest_idx) );
}