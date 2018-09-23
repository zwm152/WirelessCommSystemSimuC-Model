//
// nullspac.cpp
//

#include <iostream>
#include "nullspac.h"
#include <fstream>
extern ofstream DebugFile;

matrix_pf * NullSpace(BerlekampMatrix* b_mtx, int *r_ret)
{
  int deg = b_mtx->GetDegree();
  int prime_char = b_mtx->PrimeBase();
  matrix_pf *v = new matrix_pf(0,deg,0,deg);
  int *c = new int[deg];
  PrimeFieldElem b_val;
  int i, j, k, k1, r, s;
  bool dependence_found;

  for(i=0; i<deg; i++) c[i]=-1;
  r=0;

  for( k=0; k<deg; k++)
    {
    dependence_found = false;

    for( j=0; j<deg; j++)
      {
      if( (*b_mtx)[k][j] == 0 ) continue;
        if( c[j] >= 0 ) continue;

          //---------------------------------
          //  multiply column by -1/B[k,j]
          b_val = neg(recip( (*b_mtx)[k][j]));
          for( k1=0; k1<deg; k1++)
            (*b_mtx)[k1][j] *= b_val;

          //---------------------------------------
          // add B[k,i] times column j to column i
          // for all i .ne. j

          for( i=0; i<deg; i++)
            {
            if( (i==j) || ((*b_mtx)[k][i] == 0) ) continue;
              b_val = (*b_mtx)[k][i];
              for( k1=0; k1<deg; k1++)
                {
                (*b_mtx)[k1][i] += b_val * (*b_mtx)[k1][j];
                }
            }
          c[j] = k;
          dependence_found = true;
          break;
      } // end of loop over j
    if( !dependence_found )
      {
      for( j=0; j<deg; j++)
        {
        (*v)[r][j] = PrimeFieldElem(prime_char,0);
        for( s=0; s<deg; s++)
          {
          if( c[s] == j ) (*v)[r][j] = (*b_mtx)[k][s];
          }
        if( j==k ) (*v)[r][j] = PrimeFieldElem(prime_char,1);
        }
      r++;
      }
    } // end of loop over k
  *r_ret = r;
  return(v);
};
