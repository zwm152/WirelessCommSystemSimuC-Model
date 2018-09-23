// xor.cpp
//

#include "xor.h"
#include "math.h"

int xor(int value)
{
  int x=value;
  int result=0;

  while(x!=0)
    {
    result ^= (x&0x1);
    x >>= 1;
    }
  return( result);
}

