#ifndef _MATRIX_PF_H_
#define _MATRIX_PF_H_

#include "vector_pf.h"
//#include "rowvec_pf.h"
class rowvec_pf;
class colvec_pf;

class matrix_pf
{
protected:
  struct mrep
    {
    rowvec_pf **f;
    int refcnt;
    int orig_indx;
    int length;
    int max_indx;
    } *pM;
  int Is_Temp;
public:
  matrix_pf( void ); 
  matrix_pf( int row_orig, int nrows, 
             int col_orig, int ncols);
  matrix_pf( int nrows, 
             int ncols);
  void allocate( int row_orig, int nrows,
                  int col_orig, int ncols);
//  void DumpToStream(ostream *output_stream);
  void setrow(int row_num, PrimeFieldElem* val_vec);
   void common_ctor_proc( int row_orig, 
                   int nrows, 
                   int col_orig,
                   int ncols);
   void Resize(int nrows, int ncols);
  ~matrix_pf( void );
  int nrows(void);
  int ncols(void);
  rowvec_pf& operator[](int i);
  colvec_pf& operator*( colvec_pf &v2);
  matrix_pf& operator-=( matrix_pf &m2);
   matrix_pf& operator!();
  friend class vector_pf;
  friend class rowvec_pf;
  friend class colvec_pf;
  friend matrix_pf& operator*(matrix_pf &m1, matrix_pf &m2);
  //friend matrix_pf& gauss_jordan(matrix_pf &a, matrix_pf &b);
  ///friend vector<T>& vector<T>::operator*( matrix<T> &m2);
  //void operator=(const double_complex& right);
};
matrix_pf& transpose(matrix_pf &orig_mat);

//matrix_pf& gauss_jordan(matrix_pf &a, matrix<T> &b);
//matrix<T>& gauss_jordan(matrix<T> &a, matrix<T> &b);

//vector<double>& operator*(matrix<double> &m1, vector<double> &v2);
//matrix<double>& operator*(vector<double> &m1, vector<double> &v2);

#endif