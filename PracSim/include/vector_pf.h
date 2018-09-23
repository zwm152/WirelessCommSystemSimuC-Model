//
//  File = vector_pf.h
//

#ifndef _VECTOR_PF_H_
#define _VECTOR_PF_H_

#include "pfelem.h"
class matrix_pf;

class vector_pf
{
protected:
  struct vrep
    {
    PrimeFieldElem *f;
    int refcnt;
    int orig_indx;
    int length;
    int max_indx;
    } *pV;
  int Is_Temp;

  vector_pf(void);
public:
   vector_pf( int size);
   vector_pf( int origin, int size);
   void common_ctor_proc(int origin, int size);
   vector_pf(vector_pf &x);
   ~vector_pf(void);
   //PrimeFieldElem& operator[](int);
   friend class matrix_pf;
   friend class rowvec_pf;
   friend class colvec_pf;

};

#endif
