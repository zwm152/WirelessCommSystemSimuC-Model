//
//  File = basewav.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "basewav.h"
#include "model_graph.h"
extern int PassNumber;
extern ParmFile *ParmInput;

//======================================================
BasebandWaveform::BasebandWaveform( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<bit_t>* in_sig,
                        Signal<float>* out_sig,
                        Signal<bit_t>* bit_clock_out )
                :PracSimModel(instance_name,
                              outer_model)
{
   MODEL_NAME(BasebandWaveform);

   ENABLE_MULTIRATE;

   //---------------------------------------
   //  Read model config parms

   OPEN_PARM_BLOCK;
   Pcm_Wave_Kind = GetWaveKindParm("Pcm_Wave_Kind\0");
   BasicResults << "   " << "Pcm_Wave_Kind = " 
			    << (int)Pcm_Wave_Kind << endl;

   GET_DOUBLE_PARM( Bit_Duration );
   GET_DOUBLE_PARM( Delay_To_First_Edge );
   GET_FLOAT_PARM( Lo_Lev );
   GET_FLOAT_PARM( Hi_Lev );
   GET_FLOAT_PARM( Samps_Per_Bit );
   Bit_Clock_Enab = true;

   if(Pcm_Wave_Kind == PCM_BIPHASE){
      Samps_Per_Half_Bit = Samps_Per_Bit/2;
   }

   if(Pcm_Wave_Kind == PCM_DELAY_MOD){
      GET_BOOL_PARM(Init_Lev_Is_Lo);
      if(Init_Lev_Is_Lo)
         Init_Lev = Lo_Lev;
      else
         Init_Lev = Hi_Lev;
      Samps_Per_Half_Bit = Samps_Per_Bit/2;
   }

  //--------------------------------------
  //  Connect input and output signals

   In_Sig = in_sig;
   Out_Sig = out_sig;
   Bit_Clock_Out = bit_clock_out;

   MAKE_OUTPUT( Out_Sig );
   MAKE_OUTPUT( Bit_Clock_Out );
   MAKE_INPUT( In_Sig );

   double resamp_rate = double(Samps_Per_Bit);

   // one sample per bit at input
   SET_SAMP_INTVL( In_Sig, Bit_Duration)
   CHANGE_RATE( In_Sig, Out_Sig, resamp_rate );
   CHANGE_RATE( In_Sig, Bit_Clock_Out, resamp_rate );
}

//===================================================
BasebandWaveform::~BasebandWaveform( void ){ };

//===================================================
void BasebandWaveform::Initialize(void)
{
   Last_Bit_Val = 0.0;

   double samp_intvl = Out_Sig->GetSampIntvl();

   if(Pcm_Wave_Kind == PCM_DELAY_MOD){
      Samps_In_Curr_Bit = -1.0;
   }
   else {
      Samps_In_Curr_Bit = (Bit_Duration - 
		  Delay_To_First_Edge)/samp_intvl - 1.0;
   }
   In_First_Half = true;
   Out_Block_Size = Out_Sig->GetBlockSize();
   In_Startup_Delay = true;
   Curr_Lev = Init_Lev;
}
//====================================================
int BasebandWaveform::Execute()
{
   float last_bit_val;
   double samps_in_curr_bit, samps_per_bit;
   double samps_per_half_bit;
   float lo_lev, hi_lev, curr_lev;
   bit_t curr_bit, next_bit;
   float *waveform_out;
   bit_t *bits_in;
   bit_t *bit_clock_out_ptr;
   int is;
   int out_samp_cnt;
   int block_size;
   int out_block_size;
   bool in_first_half;
   bool transit_at_end;
   bool transit_in_center;

   last_bit_val = Last_Bit_Val;
   samps_in_curr_bit = Samps_In_Curr_Bit;
   samps_per_bit = Samps_Per_Bit;
   lo_lev = Lo_Lev;
   hi_lev = Hi_Lev;
   curr_lev = Curr_Lev;
   waveform_out = GET_OUTPUT_PTR( Out_Sig );
   bits_in = GET_INPUT_PTR( In_Sig );

   block_size = In_Sig->GetValidBlockSize();
   out_block_size = int(block_size * Samps_Per_Bit);
   Out_Sig->SetValidBlockSize(out_block_size);

   switch (Pcm_Wave_Kind) {
   case PCM_NRZ:
      for (is=0; is<out_block_size; is++){
         samps_in_curr_bit++;
         if(samps_in_curr_bit >= samps_per_bit){
            next_bit = *bits_in++;
            if(next_bit == 0)
            last_bit_val = lo_lev;
            else
            last_bit_val = hi_lev;
            samps_in_curr_bit -= samps_per_bit;
			}
        *waveform_out++ = last_bit_val;
		}
      break;
   case PCM_BIPHASE:
      samps_per_half_bit = Samps_Per_Half_Bit;
      in_first_half = In_First_Half;
      for (is=0; is<Out_Block_Size; is++){
         samps_in_curr_bit++;
         if(samps_in_curr_bit >= samps_per_bit){
            next_bit = *bits_in++;
            if(next_bit == 0)
               last_bit_val = lo_lev;
            else
               last_bit_val = hi_lev;
            samps_in_curr_bit -= samps_per_bit;
            in_first_half = true;
			}
         if( (samps_in_curr_bit >= samps_per_half_bit)
               && in_first_half ){
            last_bit_val = -last_bit_val;
            in_first_half = false;
			}
         *waveform_out++ = last_bit_val;
      }
      In_First_Half = in_first_half;
      break;
   case PCM_DELAY_MOD:
      samps_per_half_bit = Samps_Per_Half_Bit;
      transit_at_end = Transit_At_End;
      transit_in_center = Transit_In_Center;
      out_samp_cnt = 0;
      //
      // delay start of output by one bit interval, so
      // fill first bit of output with initial level
      while (In_Startup_Delay){
        samps_in_curr_bit++;
        if(samps_in_curr_bit >= samps_per_bit){
			In_Startup_Delay = false;
			next_bit = *bits_in++;
			transit_at_end = false;
			transit_in_center = false;
			}
         *waveform_out++ = curr_lev;
         out_samp_cnt++;
		}
      for (is=out_samp_cnt; is<Out_Block_Size; is++){
         samps_in_curr_bit++;
         if(samps_in_curr_bit >= samps_per_bit){
            if(transit_at_end){
               if(curr_lev == lo_lev){
                  curr_lev = hi_lev;
			   }
               else {
                  curr_lev = lo_lev;
               }
            }
            
            curr_bit = next_bit;
            next_bit = *bits_in++;

            if(curr_bit == 1){
               transit_in_center = true;
               transit_at_end = false;
            }
            else if(next_bit == 1){
               transit_in_center = false;
               transit_at_end = false;
            }
            else{
               transit_in_center = false;
               transit_at_end = true;
            }
            samps_in_curr_bit -= samps_per_bit;
         }
         if( (samps_in_curr_bit >= samps_per_half_bit)
               && transit_in_center ){
            if(curr_lev == lo_lev){
               curr_lev = hi_lev;
            }
            else{
               curr_lev = lo_lev;
            }
            transit_in_center = false;
         }
         *waveform_out++ = curr_lev;
      }
      Transit_At_End = transit_at_end;
      Transit_In_Center = transit_in_center;
      break;
   default:
      break;
   } // end of switch (Pcm_Wave_Kind)

   //------------------------------------------
   // Generate bit clock if needed

   if(Bit_Clock_Enab){
      samps_in_curr_bit = Samps_In_Curr_Bit;
      bit_clock_out_ptr = 
		           GET_OUTPUT_PTR( Bit_Clock_Out );
      for (is=0; is<Out_Block_Size; is++){
         samps_in_curr_bit++;
         if(samps_in_curr_bit >= samps_per_bit){
            samps_in_curr_bit -= samps_per_bit;
         }
         if(samps_in_curr_bit+1 >= samps_per_bit){
            *bit_clock_out_ptr++ = 1;
         }
         else {
            *bit_clock_out_ptr++ = 0;
         }
      }
   }

   Last_Bit_Val = last_bit_val;
   Samps_In_Curr_Bit = samps_in_curr_bit;
   return(_MES_AOK);
}

