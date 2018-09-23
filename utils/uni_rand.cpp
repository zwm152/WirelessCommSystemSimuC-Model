//
//  File = uni_rand.cpp
//
#include "uni_rand.h"

#define _MULT 16807
#define _MOD 2147483647
#define _HALF_MOD 1073741823
#define _FMOD 2147483647.0
#define _SQUOT 127773
#define _SREM 2836

//---------------------------------------------------
bit_t RandomBit(long *seed)
{
 long temp;

 temp = (*seed)/_SQUOT;
 *seed = _MULT * (*seed - temp * _SQUOT)
         - _SREM * temp;
 if(*seed<0) *seed += _MOD;
   if(*seed > _HALF_MOD)
      return(1);
   else
      return(0);
 }
//---------------------------------------------------
unsigned long RandomLong(long *seed)
{
   long temp;

   temp = (*seed)/_SQUOT;
   *seed = _MULT * (*seed - temp * _SQUOT)
         - _SREM * temp;
   if(*seed<0) *seed += _MOD;

   return(*seed);
 }
//--------------------------------------------- 
float UniformRandom(long *seed)
{
 long temp;
 float result;

 temp = (*seed)/_SQUOT;
 *seed = _MULT * (*seed - temp * _SQUOT)
         - _SREM * temp;
 if(*seed<0) *seed += _MOD;
 result = float(*seed/_FMOD);
 return(result);
 }
 
 
double DoubleUniformRandom(long *seed)
{
 long temp;
 double result;
 temp = (*seed)/_SQUOT;
 *seed = _MULT * (*seed - temp * _SQUOT)
         - _SREM*temp;
 if(*seed<0) *seed += _MOD;
 result = *seed/double(_FMOD);
 return(result);
 }
