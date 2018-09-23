//
//  File = trianglr.cpp
//

#include <math.h>
#include "trianglr.h"
#include "misdefs.h"


//======================================================

TriangularWindow::TriangularWindow( int length,
                                    int zero_ends )
                 :GenericWindow(length)
{
  GenerateWindow( length, zero_ends );
}
//=======================================================

void TriangularWindow::GenerateWindow( int length,
                                       int zero_ends )
{
   double denom;
   if(zero_ends)
      denom = double(Length-1);
   else
      denom = double(Length+1);

   if(length%2){
      for(int n=0; n<Half_Length; n++){
         Half_Lag_Win[n] = 1.0 - (2.0*n)/denom;
      }
   }
   else{
      for(int n=0; n<Half_Length; n++){
         Half_Lag_Win[n] = 1.0 - (2.0*n + 1.0)/denom;
      }
   }
   return;
}

