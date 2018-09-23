//
//  File = swep_tone_rcvr.cpp
//

#include <stdlib.h>
#include "parmfile.h"
#include "swep_tone_rcvr.h"
#include "misdefs.h"
#include "model_graph.h"
#include "sigplot.h"
#include "dit_pino_T.h"
#include "dit_nipo_T.h"
extern ParmFile* ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

SweptToneReceiver::SweptToneReceiver( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<float>* in_sig,
                                Signal<float>* ref_sig,
                                Signal<float>* out_sig,
                                Control<bool>* rx_enable,
                                Control<double>* inst_freq_ctl,
                                Control<double>* block_phase_ctl)
            :PracSimModel(instance_name,
                          outer_model)
{
   In_Sig = in_sig;
   Ref_Sig = ref_sig;
   Out_Sig = out_sig;

   OPEN_PARM_BLOCK;

   Response_File_Name = new char[64];
   strcpy(Response_File_Name, "\0");
   GET_STRING_PARM(Response_File_Name);
   GET_INT_PARM(Max_Num_Freqs);
   GET_INT_PARM(Num_Phases);
   GET_DOUBLE_PARM(Max_Phase_Shift);

   MAKE_OUTPUT(Out_Sig);
   MAKE_INPUT(In_Sig);
   MAKE_INPUT(Ref_Sig);

   SAME_RATE(In_Sig, Out_Sig);
   SAME_RATE(Ref_Sig, Out_Sig);

   //control output: Delay_At_Max_Corr
   //control output: Max_Corr_Angle_Out

   Inst_Freq_Ctl = inst_freq_ctl;
   Block_Phase_Ctl = block_phase_ctl;
   Rx_Enable = rx_enable;

   Response_File = new ofstream(Response_File_Name, ios::out);
}
SweptToneReceiver::~SweptToneReceiver( void ){ };

void SweptToneReceiver::Initialize(void)
{
   //int dummy_size, i;
   //double tmp_nsexp;
   //double frac_part, int_part;

   Proc_Block_Size = Out_Sig->GetBlockSize();
   Samp_Intvl = Out_Sig->GetSampIntvl();
   Max_Corr = 0.0;
   Max_Corr_Time = 0.0;
   Tone_Received = false;
   Num_Freqs = 0;
   Freqs = new double[Max_Num_Freqs];
   Magnitudes = new double[Max_Num_Freqs];
   Phases = new double[Max_Num_Freqs];
   Prev_Phase = 0.0;

   //INITIALIZATION_REPORT(BasicResults);
   return;
}

int SweptToneReceiver::Execute()
{
   using std::complex;
   float *in_sig_ptr;
   float *ref_sig_ptr;
   float *out_sig_ptr;
   float max_corr_val;
   int zero_size;
   int is;
   double corr_i, corr_q, corr_val;
   double base_phase;
   double freq_in_hz;
   double phase_increm;
   double offset_increm;
   double offset_phase;
   double phase;


   //-------------------------------------------------------
   //  Copy frequently accessed member vars into local vars
   std::complex<float> *x = X;
   std::complex<float> *y = Y;
   int proc_block_size = Proc_Block_Size;
   int full_corr_size = Full_Corr_Size;
   int ns_exp = int(Ns_Exp);
   int num_samps, num_cycles;
   double samp_intvl = Samp_Intvl;

   //----------------------------------------
   // Get pointers for input and output
   in_sig_ptr = GET_INPUT_PTR(In_Sig);
   ref_sig_ptr = GET_INPUT_PTR(Ref_Sig);
   out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);

   //----------------------------------------
   zero_size = full_corr_size - proc_block_size;
   base_phase = Block_Phase_Ctl->GetValue();
   freq_in_hz = Inst_Freq_Ctl->GetValue();
   phase_increm = freq_in_hz * Samp_Intvl;
   num_cycles = int(floor( proc_block_size * phase_increm ));
   if(num_cycles == 0)
   {
      num_samps = proc_block_size;
   }
   else
   {
      num_samps = num_cycles / phase_increm;
   }
   //offset_increm = Max_Phase_Shift/Num_Phases;
   offset_increm = 0.1/360.0;
   max_corr_val = 0.0;

   if(Rx_Enable->GetValue())
   {
      Tone_Received = true;

      in_sig_ptr = GET_INPUT_PTR(In_Sig);
      //offset_phase = base_phase - Prev_Phase - offset_idx * offset_increm;
      corr_i = 0.0;
      corr_q = 0.0;
      for( is=0; is<num_samps; is++)
      {
         corr_i += (*in_sig_ptr) * cos(TWO_PI * is * phase_increm );
         corr_q += (*in_sig_ptr) * sin(TWO_PI * is * phase_increm );
         //corr_i += (*in_sig_ptr) * cos(TWO_PI * (base_phase + is * phase_increm) );
         //corr_q += (*in_sig_ptr) * sin(TWO_PI * (base_phase + is * phase_increm) );
         in_sig_ptr++;
      }
      corr_val = corr_i * corr_i + corr_q * corr_q;
      if(corr_val > max_corr_val)
      {
         max_corr_val = corr_val;
         //max_corr_idx = offset_idx;
      }

      out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);
      offset_phase = 180.0 * atan2(corr_q, corr_i)/PI;

      for( is=0; is<proc_block_size; is++)
      {
         *out_sig_ptr = sin(TWO_PI * (offset_phase + (is*phase_increm)) );
         out_sig_ptr++;
      }

      phase = 180.0 * atan2(corr_i, corr_q)/PI;
      if( phase > 0.0 && phase < 480.0)
      {
         phase -= 360.0;
      }
      Phases[Num_Freqs] = phase;

      Freqs[Num_Freqs] = freq_in_hz;
      Magnitudes[Num_Freqs] = 20*log10(2.0*sqrt(max_corr_val)/num_samps);

      (*DebugFile) << "max_corr_val = " << max_corr_val << endl;
      (*DebugFile) << "phase of max corr = " << Phases[Num_Freqs] << endl;
      Num_Freqs++;
   }
   else
   {
      if(Tone_Received)
      {
         Tone_Received = false;
         for( is=0; is < Num_Freqs; is++)
         {
         *Response_File << Freqs[is] << ", " << Magnitudes[is] << ", " << Phases[is] << endl;
         }
      }
   }


   //---------------------
   return(_MES_AOK);
}

