//
//  File = rowvec_pf.cpp
//

//#include "complex.h"
#include "rowvec_pf.h"
#include "colvec_pf.h"
#include "matrix_pf.h"
#include "pfelem.h"

#ifdef _DEBUG
  #include <fstream>
  //#define _VEC_DEBUG 1
  extern ofstream DebugFile;
#endif

//class vector;
//------------------------------------------
//  constructor for row vector
rowvec_pf::rowvec_pf( void )
          :vector_pf()
  {
  #ifdef _VEC_DEBUG
    DebugFile << "\nshallow ctor for rowvec at " << (void*)this 
              << endl;
  #endif
  //pV->is_row_vec = 0;
  }

//------------------------------------------
rowvec_pf::rowvec_pf(int origin, int size)
          :vector_pf(origin, size)
  {
  #ifdef _VEC_DEBUG
    DebugFile << "\nctor for rowvec at " << (void*)this 
              << "  (vrep = " << (void*)pV << ")" << endl;
  #endif
  }
//==========================================
int rowvec_pf::getlen(void)
  {
  int vec_len = pV->length;
  return(vec_len);
  }

//------------------------------------------------
// transpose operator
colvec_pf& rowvec_pf::operator!( void )
{
  colvec_pf *cv = new colvec_pf(pV->orig_indx,pV->length);
  #ifdef _VEC_DEBUG
    DebugFile << "\nnew colvec at " << (void*)cv << endl;
    DebugFile << "\nrv::op!(): hook vrep "
              << (void*)pV << " to colvec " 
              << (void*)cv << endl;
  #endif
  cv->pV = pV;
  *(cv->pV)=*pV;
  cv->Is_Temp = 1;
  if(Is_Temp)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\nrv::op!(): deleting rowvec at " 
                << (void*)this << endl;
    #endif
    delete this;
    }
  return(*cv);
}

//---------------------------------------------------
//  row vector times column vector
PrimeFieldElem& rowvec_pf::operator*( colvec_pf &v2)
  {
  // get origin and length of row vector
  int v1_orig = pV->orig_indx;
  int v1_len = pV->length;

  // get origin and length of column vector
  int v2_orig = v2.pV->orig_indx;
  int v2_len = v2.pV->length;

  // alocate scalar for result
  //T *result = new T;
 // #ifdef _VEC_DEBUG
 //   DebugFile << "rv::op*(cv): new scalar alloc at "
 //             << (void*)result << endl;
 // #endif
  PrimeFieldElem *sum = new PrimeFieldElem;
  *sum = 0;
  for(int idx=0; idx<v1_len; idx++)
    {
    //sum += ((pV->f[idx+v1_orig]) * (v2.pV->f[idx+v2_orig]));
    (*sum) += ((pV->f[idx]) * (v2.pV->f[idx]));
    }
  //*result = sum;
  if(v2.Is_Temp)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\nrv::op*(cv): deleting colvec at " 
                << (void*)(&v2) << endl;
    #endif
    delete (&v2);
    }
  if(Is_Temp)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\nrv::op*(cv): deleting rowvec at " 
                << (void*)this << endl;
    #endif
    delete this;
    }
  //return(*result);
  return(*sum);
  }
//---------------------------------------------------
//  method to multiply row vector times matrix
rowvec_pf& rowvec_pf::operator*( matrix_pf &m2)
{
  // check dimensions
  int vec_orig = pV->orig_indx;
  int vec_len = pV->length;
  int row_orig = m2.pM->orig_indx;
  int nrows = m2.pM->length;
  int col_orig = ((m2.pM->f[row_orig])->pV)->orig_indx;
  int ncols = ((m2.pM->f[row_orig])->pV)->length;

  if(nrows != vec_len)
    {
    #ifdef _DEBUG
      DebugFile << "error in vector method" << endl;
    #endif
    return(*this);
    }
  //  allocate new vector for result
  rowvec_pf *v_res = new rowvec_pf(col_orig, ncols);
  v_res->Is_Temp = 1;
  #ifdef _VEC_DEBUG
    DebugFile << "rv::op*(m): new rowvec at " 
              << (void*)v_res << endl;
  #endif

  // perform multiplication and populate results vector
  PrimeFieldElem sum;
  for( int j=0; j<ncols; j++)
    {
    sum = PrimeFieldElem(0);
    for( int i=0; i<nrows; i++)
      {
      sum += ((pV->f[i]) * 
             (((m2.pM->f[i-(m2.pM->orig_indx)])->pV)->f[j]));
      }
    (v_res->pV)->f[j] = sum;
    }
  if(m2.Is_Temp)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\nrv::op*(m): deleting matrix at " 
                << (void*)(&m2) << endl;
    #endif
    delete (&m2);
    }
  if(Is_Temp)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\nrv::op*(m): deleting rowvec at " 
                << (void*)this << endl;
    #endif
    delete this;
    }
  return(*v_res);
}
//------------------------------------------------
PrimeFieldElem& rowvec_pf::operator[](int i)
  {
    return pV->f[ (((i >=(pV->orig_indx)) && (i <= pV->max_indx)) ? 
                 (i-(pV->orig_indx)) : 0)];
  }
//-----------------------------------------
// destructor
rowvec_pf::~rowvec_pf()
  {
  #ifdef _VEC_DEBUG
    DebugFile << "\ndtor for rowvec at " << (void*)this << endl;
  #endif
  //~vector();
  };
//----------------------------------------------
// assignment x = y
rowvec_pf& rowvec_pf::operator=(vector_pf &vec)
{
  vec.pV->refcnt++;
  if(--pV->refcnt == 0)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\nrv::op=(v): deleting vrep at " 
                << (void*)pV << endl;
    #endif
    delete[] pV->f;
    delete pV;
    pV = NULL;
    }
  #ifdef _VEC_DEBUG
    DebugFile << "\nrv::op=(v): hook vrep "
          << (void*)(vec.pV) << " to vector " 
          << (void*)this << endl;
  #endif
  pV = vec.pV;
  if(vec.Is_Temp)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\nrv::op=(v): deleting vector at " 
                << (void*)(&vec) << endl;
    #endif
    delete &vec;
    }
  return *this;
}

