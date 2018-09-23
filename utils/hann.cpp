//
//  File = hann.cpp
//

#include <math.h>
#include <iostream>
#include "hann.h"
#include "misdefs.h"

//======================================================
HannWindow::HannWindow( int length,
                        int zero_ends )
           :GenericWindow(length)
{
   GenerateWindow( length, zero_ends );
}
//=======================================================
void HannWindow::GenerateWindow( int length, 
                                 int zero_ends )
{
   double denom;
   if(zero_ends)
      denom = double(length-1);
   else
      denom = double(length+1);

   for(int n=0; n<Half_Length; n++){
      if(length%2) {
         Half_Lag_Win[n] = 0.5 + 0.5 * cos( TWO_PI*n/denom);
      }
      else{
         Half_Lag_Win[n] = 0.5 + 0.5 * cos( (2*n+1)*PI/denom);
      }
   }
   return;
} 

