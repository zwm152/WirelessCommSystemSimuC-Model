//
//  File = colvec_pf.cpp
//

#include "colvec_pf.h"
#include "rowvec_pf.h"
#include "matrix_pf.h"
#include "pfelem.h"

#ifdef _DEBUG
  #include <fstream>
  //#define _VEC_DEBUG 1
  extern ofstream DebugFile;
#endif

//------------------------------------------------
// transpose operator
rowvec_pf& colvec_pf::operator!( void )
{
  //rowvec<T> *rv = new rowvec<T>(pV->orig_indx,pV->length);
  rowvec_pf *rv = new rowvec_pf();
  #ifdef _VEC_DEBUG
    DebugFile << "\ncv::op!(): new rowvec at " 
              << (void*)rv << endl;
    DebugFile << "\ncv::op!(): hook vrep "
              << (void*)pV << " to rowvec " 
              << (void*)rv << endl;
  #endif
  rv->pV = pV;
  *(rv->pV)=*pV;
  ((rv->pV)->refcnt)++;
  rv->Is_Temp = 1;
  if(Is_Temp)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\ncv::op!(): deleting colvec at " 
                << (void*)this << endl;
    #endif
    delete this;
    }
  return(*rv);
}

//------------------------------------------
//  constructor for column vector
colvec_pf::colvec_pf( void )
          :vector_pf()
  {
  #ifdef _VEC_DEBUG
    DebugFile << "\nshallow ctor for colvec at " << (void*)this 
              << endl;
  #endif
  //pV->is_row_vec = 0;
  }

//------------------------------------------
//  constructor for column vector
colvec_pf::colvec_pf(int origin, int size)
          :vector_pf(origin, size)
  {
  #ifdef _VEC_DEBUG
    DebugFile << "\nctor for colvec at " << (void*)this 
              << "  (vrep = " << (void*)pV << ")" << endl;
  #endif
  }

//---------------------------------------------------
//  method to multiply column vector times row vector
matrix_pf& colvec_pf::operator*( rowvec_pf &v2)
  {
  // get origin and length of column vector
  int v1_orig = pV->orig_indx;
  int v1_len = pV->length;

  // get origin and length of row vector
  int v2_orig = v2.pV->orig_indx;
  int v2_len = v2.pV->length;

  // allocate matrix for result
  matrix_pf *m_res = 
                 new matrix_pf(v1_orig,v1_len,v2_orig,v2_len);
  m_res->Is_Temp = 1;
  #ifdef _VEC_DEBUG
    DebugFile << "\ncv::op*(rv): new matrix at " 
              << (void*)m_res << endl;
  #endif
  for(int row=0; row<v1_len; row++)
    {
    for(int col=0; col<v2_len; col++)
      {
      ((((m_res->pM)->f[row])->pV)->f[col]) = 
                                (pV->f[row]) * (v2.pV->f[col]);
      }
    }
  if(v2.Is_Temp)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\ncv::op*(rv): deleting rowvec at " 
                << (void*)(&v2) << endl;
    #endif
    delete (&v2);
    }
  if(Is_Temp)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\ncv::op*(rv): deleting colvec at " 
                << (void*)this << endl;
    #endif
    delete this;
    }
  return(*m_res);
  };
//-----------------------------------------
// destructor
colvec_pf::~colvec_pf()
  {
  #ifdef _VEC_DEBUG
    DebugFile << "\ndtor for colvec at " << (void*)this << endl;
  #endif
  //~vector();
  };

//----------------------------------------------
// assignment x = y
colvec_pf& colvec_pf::operator=(vector_pf &vec)
{
  vec.pV->refcnt++;
  if(--pV->refcnt == 0)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\ncv::op=(v): deleting vrep at " 
                << (void*)pV << endl;
    #endif
    delete[] pV->f;
    delete pV;
    }
  #ifdef _VEC_DEBUG
    DebugFile << "\ncv::op=(v): hook vrep "
          << (void*)(vec.pV) << " to vector " 
          << (void*)this << endl;
  #endif
  pV = vec.pV;
  if(vec.Is_Temp)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "\ncv::op=(v): deleting vector at " 
                << (void*)(&vec) << endl;
    #endif
    delete &vec;
    }
  return *this;
}
//---------------------------------
