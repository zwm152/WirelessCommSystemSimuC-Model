//
//  File = matrix.h
//

#ifndef _MATRIX_H_
#define _MATRIX_H_

class Matrix
{
public:
  Matrix( ){};

  virtual double* GetCol(int col_indx)=0;

protected:
  int Num_Rows;
  int Num_Cols;
};

#endif
