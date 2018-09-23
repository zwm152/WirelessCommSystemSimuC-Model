//
//  File = cyclo_partit.h
//

#ifndef _CYCLO_PARTIT_H_
#define _CYCLO_PARTIT_H_
//#include "pfelem.h"
//#include "cycpoly.h"
//#include "poly_pf.h"
//#include "berlefac.h"
//#include "deque"
using namespace std;
typedef struct{
   int *start;
   int size;
} CyclotomicCoset;

class CyclotomicPartition
{
public:

  CyclotomicPartition( int prime_field_base, int degree );
  int GetNumCosets(void);
  CyclotomicCoset *GetSpecificCoset(int coset_index);
  int GetCosetIndex(int element);
  CyclotomicCoset *Specific_Coset;

private:
  int* Coset_Start;
  int* Coset_Size;
  int* Element_Vector;
  int Num_Cosets;
  int Modulus;
};


#endif
