//
//  File = sinc.cpp
//

#include <stdlib.h>
#include <math.h>
#include "sinc.h"
#include <iostream>
#ifndef PI
  #define PI 3.1415926535897932
#endif
using namespace std;
double sinc(double x)
{
  double val;
   if (x == 0.0)
   { 
      return( 1.0 ); 
   }
   else if(ceil(x) == floor(x))
   {
      return(0.0);
   }
   else
   { 
   val = sin(PI*x)/(PI*x);
   //val = sin(x)/x;
   if(val > 2.0)
    {
    cout << "val = " << val << endl;
    exit(-1);
    }
   return(val);
   }  
}
double sinc_sqrd(double x)
{
  double val;
 if (x == 0.0)
   { return( 1.0 ); }
 else
   { 
   val = sin(PI*x)/(PI*x);
   if(val > 2.0)
    {
    cout << "val = " << val << endl;
    exit(-1);
    }
   return(val*val);
   }  
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
