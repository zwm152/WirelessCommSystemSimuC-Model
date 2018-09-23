//
//  File = uni_rand.h
//
#ifndef _UNI_RAND_H_
#define _UNI_RAND_H_
#include "typedefs.h"

bit_t RandomBit(long *seed);
unsigned long RandomLong(long *seed);
float UniformRandom(long* seed);
double DoubleUniformRandom(long* seed);

#endif      
