//
//  File = rowvec_pf.h
//

#ifndef _ROWVEC_PF_H_
#define _ROWVEC_PF_H_

#include "matrix_pf.h"
#include "vector_pf.h"


class rowvec_pf : public vector_pf
{
public:
  // constructors
  rowvec_pf( void );
  ~rowvec_pf( void);
  rowvec_pf( int origin, int size );
   int getlen(void);

  // row vector times matrix
  rowvec_pf& operator*(matrix_pf &m2);

  rowvec_pf& operator=(vector_pf &vec);
  PrimeFieldElem& operator[](int);

  // transpose operator
  colvec_pf& operator!();

  // row vector times column vector
  PrimeFieldElem& operator*(colvec_pf &v2);
  friend class colvec_pf;
  //friend rowvec<T>* transpose( colvec<T> *x);
  //friend colvec<T>& transpose( rowvec<T> &x);
};

rowvec_pf* transpose( colvec_pf* );

#endif
