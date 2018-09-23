//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = dit_nipo_T.cpp
//
//  Decimation-In-Time FFT
//
//  Naturally-ordered Input / Scrambled Output   
//

#include <math.h>
#include <complex>
#include "log2.h"
#include "bitrev.h"
#include "misdefs.h"
#include "dit_nipo_T.h"
using namespace std;

template <class T>
void IfftDitNipo( std::complex<T> *array,
                 int fft_size)
{
double trig_arg;
int log2_size;
std::complex<T> twiddle;
std::complex<T> temp;
int pts_in_lft_grp, pts_in_rgt_grp;
int stage, grp_pos, grp_cntr;
int top_node, bot_node;

log2_size = ilog2(fft_size); 

pts_in_rgt_grp = fft_size;
for( stage=1; stage <=log2_size; stage++)
  {   
   grp_cntr = -1;
   
   pts_in_lft_grp = pts_in_rgt_grp;  // set pts_in_left_dft = N/(2**(stage-1))
   pts_in_rgt_grp /= 2;               // set pts_in_right_dft = N/(2**stage)

   for( grp_pos =0; grp_pos < fft_size; grp_pos += pts_in_lft_grp)
     {
      grp_cntr++;
                
      trig_arg = (TWO_PI*bitrev(grp_cntr, log2_size-1))/fft_size;
      twiddle = std::complex<T>(float(cos(trig_arg)), float(sin(trig_arg)));
                                  
      for( top_node = grp_pos; top_node < grp_pos+pts_in_rgt_grp; 
                               top_node++)
        {                              
         bot_node = top_node + pts_in_rgt_grp;
         temp = array[bot_node] * twiddle;
         array[bot_node] = array[top_node] - temp;
         array[top_node] += temp;
        }  // end of loop over top_node
        
     } // end of loop over grp_pos
  } // end of loop over stage
return;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
template <class T>
void FftDitNipo( complex<T> *array,
                 int fft_size)
{
   double trig_arg;
   std::complex<T> twiddle, temp;
   int pts_in_lft_grp, pts_in_rgt_grp;
   int stage, grp_pos, grp_cntr;
   int log2_size, top_node, bot_node;

   log2_size = ilog2(fft_size); 
   pts_in_rgt_grp = fft_size;
   for( stage=1; stage <=log2_size; stage++){   
      grp_cntr = -1;

      // set pts_in_left_dft = N/(2**(stage-1))
      pts_in_lft_grp = pts_in_rgt_grp;  
      
      // set pts_in_right_dft = N/(2**stage)
      pts_in_rgt_grp /= 2;

      for(  grp_pos =0; 
            grp_pos < fft_size; 
            grp_pos += pts_in_lft_grp){
         grp_cntr++;

         trig_arg = (TWO_PI * bitrev(grp_cntr, 
                     log2_size-1))/fft_size;
         twiddle = complex<T>( float(cos(trig_arg)), 
                               float(-sin(trig_arg)));

         for(  top_node = grp_pos; 
               top_node < grp_pos+pts_in_rgt_grp; 
               top_node++){                              
            bot_node = top_node + pts_in_rgt_grp;
            temp = array[bot_node] * twiddle;
            array[bot_node] = array[top_node] - temp;
            array[top_node] += temp;
         }
      } // end of loop over grp_pos
   } // end of loop over stage
   return;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
template void IfftDitNipo( std::complex<float> *array,
                            int fft_size);

template void FftDitNipo( std::complex<float> *array,
                           int fft_size);
