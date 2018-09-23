//
//  File = bitrev.cpp
//

#include "bitrev.h"

int bitrev( int input_value, int num_bits)
 {
  int bit_pos, work_val, output_value;
  work_val = input_value; 
  output_value = 0;
  for (bit_pos=0; bit_pos<num_bits; bit_pos++){
     output_value = (output_value<<1) | (int(0x1) & work_val);
     work_val >>= 1;
  }
  return(output_value);
 }
 
