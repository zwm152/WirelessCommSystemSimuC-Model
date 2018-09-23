//======================================================
//
//  File = butterworth_proto.cpp
//
//  Prototype Butterworth Filter
//

#include <math.h>
#include "misdefs.h"
#include "parmfile.h"
#include "butterworth_proto.h"
#include "filter_types.h"
extern ParmFile ParmInput;

extern ofstream *DebugFile;
using std::complex;

//======================================================
//  constructor

ButterworthPrototype::
            ButterworthPrototype( int filt_order )
                  :AnalogFilterPrototype( filt_order )
{

   //======================================================
   //  Computes poles for normalized Butterworth prototype

   double x;

   Pole_Locs = new complex<double>[Filt_Order];
   Num_Poles = Filt_Order;
   Zero_Locs = new std::complex<double>[1];
   Num_Zeros = 0;
   H_Zero = 1.0;

   if(Filt_Order%2){ 
      //order is odd
      Num_Biquad_Sects = (Filt_Order-1)/2;
      Pole_Locs[Num_Biquad_Sects] = 1.0;
      Real_Pole = 1.0;
   }
   else {
      Num_Biquad_Sects = Filt_Order/2;
   }
   B1_Coef = new double[Num_Biquad_Sects];
   B0_Coef = new double[Num_Biquad_Sects];

   for( int k=1; k<=Num_Biquad_Sects; k++){
      x = PI * (Filt_Order + (2*k)-1) / (2*Filt_Order);
      Pole_Locs[k-1] = complex<double>( cos(x), sin(x));
#ifdef _DEBUG
      *DebugFile << "LP Proto pole " << k-1 << " at ( "
         << Pole_Locs[k-1].real() << ", "
         << Pole_Locs[k-1].imag() << " )" 
         << endl;
#endif
      Pole_Locs[Filt_Order-k] = 
                  complex<double>( cos(x), -sin(x));
#ifdef _DEBUG
      *DebugFile << "LP Proto pole " 
                 << Filt_Order-k << " at ( "
                 << Pole_Locs[Filt_Order-k].real()
                 << ", " 
                 << Pole_Locs[Filt_Order-k].imag() 
                 << " )" << endl;
#endif
      B1_Coef[k-1] = -2.0* (Pole_Locs[k-1].real());
      B0_Coef[k-1] = 1.0;
   }
}
//==========================================================
ButterworthPrototype::~ButterworthPrototype()
{
}