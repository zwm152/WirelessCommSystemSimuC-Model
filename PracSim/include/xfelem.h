//
//  File = xfelem.h
//

#ifndef _XFELEM_H_
#define _XFELEM_H_


#include <iostream>
#include "pfelem.h"
using namespace std;
//#include "galfield.h"
class GaloisField;


class ExtenFieldElem
{
public:

  ExtenFieldElem(void);
  ExtenFieldElem(GaloisField*);
  ExtenFieldElem(GaloisField*, int value);
  bool EqualsZero();
  //void operator=(const int);
  void operator=(const ExtenFieldElem&);
  friend ExtenFieldElem operator+( const ExtenFieldElem,
                               const ExtenFieldElem );
  friend ExtenFieldElem operator-( const ExtenFieldElem,
                               const ExtenFieldElem );
  friend ExtenFieldElem operator*( const ExtenFieldElem,
                               const ExtenFieldElem );
  //friend ExtenFieldElem operator/( const ExtenFieldElem,
  //                             const ExtenFieldElem );
  friend ostream& operator<< (ostream&, const ExtenFieldElem&);
  friend class GaloisField;

  PrimeFieldElem* Value;
  //ExtenFieldElem* Value;

private:
  GaloisField* Field;
  int FieldDegree;
};

inline void ExtenFieldElem::operator=(const ExtenFieldElem &right)
  {
  Field = right.Field;
  FieldDegree = right.FieldDegree;
  Value = new PrimeFieldElem[FieldDegree];
  for(int n=0; n<FieldDegree; n++)
    {
    Value[n]=right.Value[n];
    }
}


#endif //_XFELEM_H_
