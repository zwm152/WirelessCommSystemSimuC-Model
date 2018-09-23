//
//  File = toeplitz.cpp
//

//#include <stdlib.h>
#include "toeplitz.h"

ToeplitzMatrix::ToeplitzMatrix( void )
  {
  return;
  }

double* ToeplitzMatrix::GetCol(int col_indx)
  {
  // right now this only returns column 1
  int indx;
  double *col_vect, *src_ptr, *dest_ptr;
  col_vect = new double[Num_Rows];
  dest_ptr = col_vect;
  src_ptr = Herm_Toep_Col_1;
  for(indx=0; indx<Num_Rows; indx++)
    {
    *dest_ptr = *src_ptr;
    dest_ptr++;
    src_ptr++;
    }
  return(col_vect);
  }

