//
//  File = colvec_pf.h
//

#ifndef _COLVEC_PF_H_
#define _COLVEC_PF_H_

//#include "complex.h"
#include "colvec_pf.h"
#include "vector_pf.h"
#include "rowvec_pf.h"

//=======================================
//=======================================
class colvec_pf : public vector_pf
{
public:
  // constructor
  colvec_pf( void );
  colvec_pf( int origin, int size );
  ~colvec_pf( void );

  // column vector time row vector
  matrix_pf& operator*(rowvec_pf &v2);

  colvec_pf& operator=(vector_pf &vec);

  // transpose operator
  rowvec_pf& operator!();

  friend class rowvec_pf;
  //friend rowvec<T>* transpose( colvec<T> *x);
  //friend colvec<T>& transpose( rowvec<T> &x);
};


rowvec_pf* transpose( colvec_pf* );

#endif
