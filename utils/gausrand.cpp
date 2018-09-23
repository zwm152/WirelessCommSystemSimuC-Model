//
//  File = gausrand.cpp
//

#include <math.h>
#include "uni_rand.h"
#include "gausrand.h"
 
void GaussRandom(long *seed, float *result)
{
 double x1, x2, g1; 
 double XA, XB, big_S;
 double radical;

 big_S=1.0;
 while(big_S >= 1.0){
   XA = DoubleUniformRandom(seed);
   XB = DoubleUniformRandom(seed);
   x1 = 1.0 - 2.0 * XA;
   x2 = 1.0 - 2.0 * XB;
 big_S = x1*x1 + x2*x2;
 }
 radical = sqrt(-2.0*log(big_S)/big_S);
 g1 = x1 * radical;
 *result = float(g1);
}
//------------------------------------------------
void GaussRandom(long *seed, double *result)
{
 double x1, x2, g1; 
 double XA, XB, big_S;
 double radical;

 big_S=1.0;
 while(big_S >= 1.0){
   XA = DoubleUniformRandom(seed);
   XB = DoubleUniformRandom(seed);
   x1 = 1.0 - 2.0 * XA;
   x2 = 1.0 - 2.0 * XB;
   big_S = x1*x1 + x2*x2;
 }
 radical = sqrt(-2.0*log(big_S)/big_S);
 g1 = x1 * radical;
 *result = g1;
}
//------------------------------------------------
void GaussRandom(long *seed, std::complex<double> *result)
{
 double x1, x2, g1, g2; 
 double XA, XB, big_S;
 double radical;

 big_S=1.0;
 while(big_S >= 1.0){
   XA = DoubleUniformRandom(seed);
   XB = DoubleUniformRandom(seed);
   x1 = 1.0 - 2.0 * XA;
   x2 = 1.0 - 2.0 * XB;
   big_S = x1*x1 + x2*x2;
 }
 radical = sqrt(-2.0*log(big_S)/big_S);
 g1 = x1 * radical;
 g2 = x2 * radical;
 *result = std::complex<double>(g1, g2);
}
//------------------------------------------------
void GaussRandom(long *seed, std::complex<float> *result)
{
 double x1, x2;
 float g1, g2; 
 double XA, XB, big_S;
 double radical;

 big_S=1.0;
 while(big_S >= 1.0){
   XA = DoubleUniformRandom(seed);
   XB = DoubleUniformRandom(seed);
   x1 = 1.0 - 2.0 * XA;
   x2 = 1.0 - 2.0 * XB;
   big_S = x1*x1 + x2*x2;
 }
 radical = sqrt(-2.0*log(big_S)/big_S);
 g1 = float(x1 * radical);
 g2 = float(x2 * radical);
 *result = std::complex<float>(g1, g2);
}
