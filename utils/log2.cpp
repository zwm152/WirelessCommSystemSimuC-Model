//
//  File = log2.cpp
//

#include "log2.h"

int ilog2(int value_inp)
{
 int log_val;
 int value; 

 log_val = 0;
 value = value_inp;
 while(value >1){
    value /=2;
    log_val++; 
   }  
 value = 1;
 int i;
 for(i=1; i<=log_val ;i++){ 
    value *= 2;
   }
 if(value != value_inp) log_val = -1;
 return(log_val);  
}
