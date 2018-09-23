//
//  File = samp_curve.cpp
//

#include <stdlib.h>
#include <math.h>
#include "samp_curve.h"
#include "misdefs.h"
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

SampledCurve::SampledCurve(char* data_fname )
{
   int i;

   Data_File = new ifstream(data_fname, ios::in);
   *Data_File >> Num_Samps;

   Ordinate_Vals = new double[Num_Samps+1];
   Ordinate_Vals[0] = 0.0;

   for( i=1; i<=Num_Samps; i++){
      *Data_File >> Ordinate_Vals[i];
   }
}
//======================================================
SampledCurve::~SampledCurve( void )
{ 
   delete[] Ordinate_Vals;
};
//======================================================
double SampledCurve::GetValue(double abscissa)
{
   double whole;
   double frac;
   int index;
   double value;

   frac = modf(abscissa, &whole);
   index = int(whole);
   if(index > Num_Samps-1){
      value = Ordinate_Vals[Num_Samps-1];
   }
   else{
      value = frac*Ordinate_Vals[index+1] + 
               (1.0-frac)*Ordinate_Vals[index];
   }
   return(value);
}

