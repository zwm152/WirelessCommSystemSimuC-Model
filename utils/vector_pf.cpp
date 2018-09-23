//
//  File = vector_pf.cpp
//

#include "vector_pf.h"
#include "matrix_pf.h"
#include "pfelem.h"

#ifdef _DEBUG
  #include <fstream>
  //#define _VEC_DEBUG 1
  extern ofstream DebugFile;
#endif

//------------------------------------------
vector_pf::vector_pf( void )
  {
  #ifdef _VEC_DEBUG
    DebugFile << "\nctor for vector(void) at " 
              << (void*)this << endl;
  #endif
  Is_Temp = 0;
  };

//------------------------------------------
vector_pf::vector_pf(int origin, int size)
  {
  if( size <= 0)
      cout << "illegal vector dimension" << endl;
  pV = new vrep;
  pV->orig_indx = origin;
  pV->length = size;
  pV->max_indx = origin + size -1;
  pV->f = new PrimeFieldElem[size];
  #ifdef _VEC_DEBUG
    DebugFile << "v::v(i,i): array " << size << " long alloc at "
              << (void*)(pV->f) << endl;
  #endif
  for(int i=0; i<size; i++)
    pV->f[i] = PrimeFieldElem(0);
  pV->refcnt = 1;
  Is_Temp = 0;
  };

//-----------------------------------------
// destructor
vector_pf::~vector_pf()
  {
  #ifdef _VEC_DEBUG
    DebugFile << "\ndtor for vector at " << (void*)this << endl;
  #endif
  if(pV!=NULL)
    {
    #ifdef _VEC_DEBUG
      DebugFile << "refcnt = " << (pV->refcnt) << endl;
    #endif
    if(--pV->refcnt == 0)
      {
      #ifdef _VEC_DEBUG
        DebugFile << "\nv::~v(): deleting elem array at "
                  << (void*)(pV->f) << endl;
      #endif
      delete[] pV->f;
      #ifdef _VEC_DEBUG
        DebugFile << "\nv::~v(): deleting vrep at "
                  << (void*)pV << endl;
      #endif
      delete pV;
      pV = NULL;
      }
    }
  };
//------------------------------------------------------
vector_pf::vector_pf( vector_pf &x)
{
  #ifdef _VEC_DEBUG
    DebugFile << "in copy constructor v::v(v)" << endl;
  #endif
  x.pV->refcnt++;
  pV=x.pV;
}

#if 0
PrimeFieldElem& vector_pf::operator[](int i)
  {
    return pV->f[ (((i >=(pV->orig_indx)) && (i <= pV->max_indx)) ? 
                 (i-(pV->orig_indx)) : 0)];
  }
#endif
