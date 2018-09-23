//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = numinteg.cpp
//
//  simulation of analog all-pole filter
//

#include <math.h>
#include "numinteg.h"

//======================================================
//  constructor that initializes the integrator
//------------------------------------------------------

NumericInteg::NumericInteg( double delta_t, double alpha )
{
 Delta_T = delta_t;
 Alpha = alpha;
 Integ_Mem = 0.0;
 return;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++      


//======================================================
//
//------------------------------------------------------

double NumericInteg::Integrate( double input )
{ 
Integ_Mem = Alpha * Integ_Mem + (Delta_T * input);
return(Integ_Mem);
}
