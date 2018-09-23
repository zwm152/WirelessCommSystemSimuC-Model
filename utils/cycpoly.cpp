// cycpoly.cpp
//

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "math.h"
//#include "galfield.h"
#include "poly_int.h"
#include "cycpoly.h"
#include "stdlib.h"

extern ofstream DebugFile;

CyclotomicPoly::CyclotomicPoly(int e_val, int prime_char)
               :PolyOvrIntegers(0,1)
{
	int exp_val, d_val;
  this->DumpToStream(&cout);
  for( d_val=1; d_val<=e_val; d_val++)
    {
    if((e_val%d_val)!=0) continue;  // skip values of d that don't
                                    // divide e
      exp_val = Moebius(e_val/d_val);
    if( exp_val != 1 ) continue;  // build entire numerator first
                                  // do a second pass below to
                                  // pick up denominator factors
      (*this) *= PolyOvrIntegers( d_val, 1);
    //this->DumpToStream(&DebugFile);
    }
  //- - - - - - - - -
  for( d_val=1; d_val<=e_val; d_val++)
    {
    if((e_val%d_val)!=0) continue;  // skip values of d that don't
                                    // divide e
      exp_val = Moebius(e_val/d_val);
    if( exp_val != -1 ) continue;  // process only denominator
                                   // factors this time
      (*this) /= PolyOvrIntegers( d_val, 1);
    }
}
int Moebius(int m)
{
   int p_factors[20];
   int fac_cnt = 0;
   bool fac_found, dup_facs;
   int m_work, n_stop, fac_idx;

   if( m == 1 ) return(1);

   dup_facs = false;
   m_work = m;
   n_stop = m_work;
   for(;;){
      fac_found = false;
      for(int n=2; n<=m_work-1; n++){
         if((m_work%n) != 0 ) continue;
         // else
         // n is a factor of m_work
         // check for duplicate factor
         for( fac_idx=0; fac_idx<fac_cnt; fac_idx++){
            if(n == p_factors[fac_idx]) dup_facs = true;
         }
         p_factors[fac_cnt] = n;
         m_work /= n;
         fac_found = true;
         fac_cnt++;
         break;
      }
      if(!fac_found) break;
      fac_found = false;
   }
   p_factors[fac_cnt] = m_work;
   // check for duplicate factor
   for( fac_idx=0; fac_idx<fac_cnt; fac_idx++)
   {
      if(m_work == p_factors[fac_idx]) dup_facs = true;
   }
   fac_cnt++;

   if( (!dup_facs) && ((fac_cnt%2)==0)) return(1);
   if( (!dup_facs) && ((fac_cnt%2)!=0)) return(-1);
   return(0);
}

