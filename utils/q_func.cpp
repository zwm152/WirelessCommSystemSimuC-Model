//
//  File = q_func.cpp
//

//#include <stdlib.h>
#include <math.h>
#include "q_func.h"
#ifndef PI
  #define PI 3.1415926535897932
#endif
#define ROOT_2 1.414213562

double q_func(double x)
{
  double val;
  if(x ==0)
    {
    val = 0.5;
    }
  else
    {
    val = 0.5 * erfc(x/ROOT_2);
    }
  return(val);
}

double erfc(double x)
{
  double w, y, z;
  z = fabs(x);
  w = 1.0 / (1.0 + 0.5*z);
  y = w*exp(-z*z-1.26551223+w*(1.00002368+w*(0.37409196+
      w*(0.09678418+w*(-0.18628806+w*(0.27886807+w*(-1.13520398+
      w*(1.48851587+w*(-0.82215223+w*0.17087277)))))))));
  if(x<0.0) y = 2.0 - y;
  return(y);
}

double marcum_q(double a, double t)
{
  #define prec 0.01
  double result;
  double ct, ca, et, g, k, gnew;
  int nn, n;
  if( (a>20.0) && t>10.0*(t-a) )
    {
    //result = 0.5 * erfc((t-a)/ROOT_2);
    result = 0.0;
    }
  else
    {
    ct = t*t/2.0;
    ca = a*a/2.0;
    et = exp(-ct);
    g = 1.0 - et;
    k = exp(-ca);
    gnew = et;
    result = 1.0 - g*k;
    nn = 1+int(ceil(0.5*a*t));
    for(n=1; n<=nn; n++)
      {
      gnew = gnew * ct/double(n);
      g -= gnew;
      k *= (ca/double(n));
      result -= g*k;
      }
    while( (g*k/1.0 - (0.5*a*t/n)*(0.5*a*t/n)) > prec*result)
      {
      n++;
      gnew *= ct/double(n);
      g -= gnew;
      k *= ca/double(n);
      result -= g*k;
      }
    } // end of else clause
  return(result);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
