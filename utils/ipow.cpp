//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = ipow.cpp
//
//


//-----------------------------------------------------
// raise double to an integer power

double ipow(double x, int m)
{
 int i;
 double result;
 result = 1.0;
 for(i=1; i<=m; i++)
   {
    result *= x;
   }
 return(result);
}
