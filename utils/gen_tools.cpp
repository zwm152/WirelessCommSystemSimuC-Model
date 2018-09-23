// gen_tools.cpp
//

#include "gen_tools.h"

int TrailingZeroCount(int value)
{
  int result=0;
  int work = value;
  int lsb = work & 0x1;

  while( lsb == 0)
    {
    result++;
    work >>=1;
    lsb = work & 0x1;
    }
  return( result);
}

int ActiveBitCount(int value)
{
  int num_trail_zeros;
  num_trail_zeros = TrailingZeroCount( value);

  int work = value >> num_trail_zeros;
  int result = 0;

  while( work != 0 )
    {
    result++;
    work >>= 1;
    }
  return(result);
}

int ReverseBits( int value, int num_bits)
{
  int work = value;
  int result =0;

  for(int i=0; i<num_bits; i++)
    {
    result <<=1;
    result |= (work & 0x1);
    work >>=1;
    }
  return(result);
}

