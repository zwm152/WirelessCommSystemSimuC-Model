//
//  File = toeplitz.h
//

#ifndef _TOEPLITZ_H_
#define _TOEPLITZ_H_

//#include "complex.h"
#include "matrix.h"

class ToeplitzMatrix : public Matrix
{
public:
  ToeplitzMatrix( );

  double* GetCol(int col_indx);

protected:
  double *Herm_Toep_Col_1;

};

#endif
