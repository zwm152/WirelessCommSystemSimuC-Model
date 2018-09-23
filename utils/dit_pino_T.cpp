//======================================================
//
//  File = dit_pino_T.cpp
//
//  Decimation-In-Time FFT
//
//  Permuteded Input / Naturally-ordered Output
//

#include <math.h>
#include "misdefs.h"
#include "log2.h"
#include "cbitrev_T.h"
#include "dit_pino_T.h"

template <class T>
void IfftDitPino( std::complex<T> *array,
                  int fft_size)
{
   double trig_arg;
   int log2_size;
   std::complex<T> twiddle, w_fact;
   std::complex<T> temp;
   int pts_in_left_dft, pts_in_right_dft;
   int stage, bfly_pos;
   int top_node, bot_node;
   log2_size = ilog2(fft_size);

   pts_in_right_dft = 1;
   for( stage=1; stage <=log2_size; stage++) { 

      // set pts_in_left_dft = 2**(stage-1)
      pts_in_left_dft = pts_in_right_dft;

      // set pts_in_right_dft = 2**stage
      pts_in_right_dft *= 2;

      twiddle = std::complex<T>(1.0, 0.0);
      trig_arg = PI/pts_in_left_dft;  
      w_fact = std::complex<T>(  T(cos(trig_arg)), 
                                 T(sin(trig_arg)));

      for(  bfly_pos =0; 
            bfly_pos < pts_in_left_dft; 
            bfly_pos++){                             
         for(  top_node = bfly_pos; 
               top_node<fft_size; 
               top_node += pts_in_right_dft){                              
            bot_node = top_node + pts_in_left_dft;
            temp = array[bot_node] * twiddle;
            array[bot_node] = array[top_node] - temp;
            array[top_node] += temp;
         }
         twiddle *= w_fact;
      }
   }
   return;
}
//======================================================
template <class T>
void FftDitPino( std::complex<T> *array,
                 int fft_size)
{
   double trig_arg;
   int log2_size;
   std::complex<T> twiddle, w_fact;
   std::complex<T> temp;
   int pts_in_left_dft, pts_in_right_dft;
   int stage, bfly_pos;
   int top_node, bot_node;
   log2_size = ilog2(fft_size);

   pts_in_right_dft = 1;
   for( stage=1; stage <=log2_size; stage++) {

      // set pts_in_left_dft = 2**(stage-1)
      pts_in_left_dft = pts_in_right_dft; 

      // set pts_in_right_dft = 2**stage
      pts_in_right_dft *= 2;               

      twiddle = std::complex<T>(1.0, 0.0);
      trig_arg = PI/pts_in_left_dft;  
      w_fact = std::complex<T>(  T(cos(trig_arg)), 
                                 T(-sin(trig_arg)));

      for(  bfly_pos =0; 
            bfly_pos < pts_in_left_dft; 
            bfly_pos++) { 
         for(  top_node = bfly_pos; 
               top_node<fft_size; 
               top_node += pts_in_right_dft){
            bot_node = top_node + pts_in_left_dft;
            temp = array[bot_node] * twiddle;
            array[bot_node] = array[top_node] - temp;
            array[top_node] += temp;
         }
         twiddle *= w_fact;
      }
   }
   return;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
template void IfftDitPino( std::complex<double> *array,
                            int fft_size);
template void IfftDitPino( std::complex<float> *array,
                            int fft_size);

template void FftDitPino( std::complex<double> *array,
                           int fft_size);
template void FftDitPino( std::complex<float> *array,
                           int fft_size);
