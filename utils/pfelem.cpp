// pfelem.cpp
//

#include <iostream>
#include "pfelem.h"

PrimeFieldElem::PrimeFieldElem(void)
{
  Modulus=1;
  Value=0;
}
PrimeFieldElem::PrimeFieldElem(int modulus)
{
  Modulus=modulus;
  Value=0;
}

PrimeFieldElem::PrimeFieldElem(int modulus, int value)
{
  Modulus=modulus;
  Value = value%modulus;
}

PrimeFieldElem neg( PrimeFieldElem _e1)
{
  return PrimeFieldElem(_e1.Modulus,(_e1.Modulus-_e1.Value));
}

PrimeFieldElem recip( PrimeFieldElem _e1)
{
  int ret_val=0;
  for(int i=0; i<_e1.Modulus; i++)
    {
    if( ((_e1.Value*i) % _e1.Modulus) == 1 ) ret_val = i;
    }
  return PrimeFieldElem(_e1.Modulus,ret_val);
}

PrimeFieldElem operator/( const PrimeFieldElem _e1, const PrimeFieldElem _e2)
  {return PrimeFieldElem(_e1.Modulus,(_e1.Value * recip(_e2).Value ));}


ostream& operator<< ( ostream& s, const PrimeFieldElem& element)
{
  s << element.Value;
  return s;
}
