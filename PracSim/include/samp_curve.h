//
//  File = samp_curve.h
//

#ifndef _SAMP_CURVE_H_
#define _SAMP_CURVE_H_

#include <fstream>
using namespace std;

class SampledCurve
{
public:
   SampledCurve(char* data_fname);

   ~SampledCurve(void);
   void Initialize(void);
   double GetValue(double abscissa);

private:
   int Num_Samps;
   ifstream *Data_File;
   double *Ordinate_Vals;
   bool Bypass_Enabled;
};

#endif
