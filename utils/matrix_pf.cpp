//
//  File = matrix_pf.cpp
//

#include <error.h>
#include "matrix_pf.h"
#include "colvec_pf.h"
#include "pfelem.h"

#ifdef _DEBUG
  #include <fstream>
  extern ofstream DebugFile;
  //#define _MTX_DEBUG 1
#endif

//---------------------------------------------
//  constructor
matrix_pf::matrix_pf( void )
{
};
//---------------------------------------------
//  constructor
matrix_pf::matrix_pf( int row_orig, 
                   int nrows, 
                   int col_orig,
                   int ncols)
  {
  allocate( row_orig, nrows, col_orig, ncols);
  };
//======================================================
void matrix_pf::allocate(  int row_orig, 
                           int nrows, 
                           int col_orig,
                           int ncols)
  {
  if( (nrows <= 0) || (ncols <=0))
      cout << "illegal matrix dimension" << endl;
  pM = new mrep;
  #ifdef _MTX_DEBUG
  DebugFile << "\nctor for matrix at " << this 
            << "   (mrep = " << (void*)pM << ")" << endl;
  #endif
  pM->length = nrows;
  pM->orig_indx = row_orig;
  pM->max_indx = row_orig + nrows - 1;
  pM->f = new rowvec_pf*[nrows];
  #ifdef _MTX_DEBUG
    DebugFile << "v::v(i,i): ptr array " << nrows 
              << " long alloc at "
              << (void*)(pM->f) << endl;
  #endif
  for(int i=0; i<nrows; i++)
    pM->f[i] = new rowvec_pf( col_orig, ncols);
  pM->refcnt = 1;
  Is_Temp = 0;
  };

//----------------------------------------------
matrix_pf::~matrix_pf(void)
{
  //rowvec<T> *row_ptr;

  #ifdef _MTX_DEBUG
  DebugFile << "\ndtor for matrix at " << (void*)this << endl;
  #endif
  if( --pM->refcnt == 0)
    {
    int nrows = pM->length;
    for(int i=0; i<nrows; i++)
            delete pM->f[i];
    delete pM->f;
    #ifdef _MTX_DEBUG
    DebugFile << "\nm::~m(): deleting mrep at " 
              << (void*)pM << endl;
    #endif
    delete pM;
    }
}
//----------------------------------------------
// set row of values

void matrix_pf::setrow(int row_num, PrimeFieldElem *val_vec)
{
  int ncols = ((pM->f[pM->orig_indx])->pV)->length;
  for(int j=0; j<ncols; j++)
    {
    ((pM->f[row_num])->pV)->f[j] = val_vec[j];
    }
}
//====================================================
#if 0
void matrix_pf::DumpToStream( ostream* output_stream)
{
  int nrows = pM->length;
  int ncols = ((pM->f[pM->orig_indx])->pV)->length;
  (*output_stream) << "matrix<T> Dump:" << endl;
  for( int i=0; i<nrows; i++)
    {
    for( int j=0; j<ncols; j++)
      {
      (*output_stream) << (((pM->f[i])->pV)->f[j]) << "  ";
      }
    (*output_stream) << endl;
    }
}
#endif
//----------------------------------------------
// row extraction
rowvec_pf& matrix_pf::operator[](int i)
{
  return *(pM->f[ (((i>=(pM->orig_indx)) && (i<= pM->max_indx)) ? 
                 (i-(pM->orig_indx)) : 0)]);
}

//--------------------------------------------------
//  post-multiply matrix by a column vector
colvec_pf& matrix_pf::operator*( colvec_pf &v2)
{
  // check dimensions
  int row_orig = pM->orig_indx;
  int nrows = pM->length;
  int col_orig = ((pM->f[pM->orig_indx])->pV)->orig_indx;
  int ncols = ((pM->f[pM->orig_indx])->pV)->length;
  int vec_orig = v2.pV->orig_indx;
  int vec_len = v2.pV->length;
  
  if(ncols != vec_len)
    {
    cout << "error in matrix method" << endl;
    return( v2 );
    }

  // allocate new vector for result
  colvec_pf *v_res = new colvec_pf(row_orig, nrows);
  #ifdef _MTX_DEBUG
  DebugFile << "\nm::op*(cv): new colvec at "
            << (void*)v_res << endl;
  #endif
  v_res->Is_Temp = 1;

  // perform multiplication and populate results vector
  PrimeFieldElem sum;
  for(int i=0; i<nrows; i++)
    {
    sum = PrimeFieldElem(0);
    for(int j=0; j<vec_len; j++)
      {
      //sum += ((v2.pV->f[j]) * (((pM->f[i-(pM->orig_indx)])->pV)->f[j]));
      sum += ((v2.pV->f[j]) * (((pM->f[i])->pV)->f[j]));
      }
    (v_res->pV)->f[i] = sum; 
    }
  if(v2.Is_Temp)
    {
    #ifdef _MTX_DEBUG
    DebugFile << "\nm::op*(cv): deleting colvec at " 
              << (void*)(&v2) << endl;
    #endif
    delete (&v2);
    }
  if(Is_Temp)
    {
    #ifdef _MTX_DEBUG
    DebugFile << "\nm::op*(cv): deleting matrix at " 
              << (void*)this << endl;
    #endif
    delete this;
    }
  return (*v_res);
}
//--------------------------------------------------
//  do element-by-element subtraction
matrix_pf& matrix_pf::operator-=(matrix_pf &m2)
  {
  int nrows = pM->length;
  int ncols = ((pM->f[pM->orig_indx])->pV)->length;
  for(int i=0; i<nrows; i++)
    {
    for(int j=0; j<ncols; j++)
      {
      ((pM->f[i])->pV)->f[j] -= (((m2.pM)->f[i])->pV)->f[j];
      }
    }
  if(m2.Is_Temp)
    {
    #ifdef _MTX_DEBUG
    DebugFile << "\nm::op-=(m): deleting matrix at " 
              << (void*)(&m2) << endl;
    #endif
    delete (&m2);
    }
  return(*this);
  }
//template matrix<double>;
//template matrix<complex>;
//template matrix< PrimeFieldElem() >;
