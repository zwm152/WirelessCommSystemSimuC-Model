// isprime.cpp
//

#include "isprime.h"
#include <iostream>
#include "math.h"

bool ValueIsPrime(int value)
{
  int trial, max_factor;
  max_factor = int(sqrt(double(value)));
  if( value == 2 ) return true;
  if((value%2) == 0)
    {
    return false;
    }
  else
    {
    for(trial=3; trial<=max_factor; trial+=2)
      {
      if((value%trial) == 0) return false;
      }
    }
  return true;
}

