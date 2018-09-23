//
//  File = raylrand.cpp
//

#include <math.h>
#include "uni_rand.h"
#include "gausrand.h"
 
void RayleighRandom(long *seed, float *result)
{
   std::complex<float> cmpx_result;
   GaussRandom(seed, &cmpx_result);
   *result = float(std::abs(cmpx_result) - 1.2533);
 //*result = float(sqrt(-log(DoubleUniformRandom(seed))));
}
