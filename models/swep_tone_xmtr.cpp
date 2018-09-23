//
//  File = swep_tone_xmtr.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "swep_tone_xmtr.h"
//#include "uni_rand.h"
#include "misdefs.h"
#include "parmfile.h"
#include "model_graph.h"
#include "sigstuff.h"
#include "syst_graph.h"

extern int PassNumber;
extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif


//======================================================

SweptToneTransmitter::SweptToneTransmitter( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float>* out_sig,
                    Control<bool> *rx_enable,
                    Control<double> *inst_freq_ctl,
                    Control<double> *block_phase_ctl)
        :PracSimModel( instance_name,
                        outer_model )
{
  MODEL_NAME(SweptToneTransmitter);
  Out_Sig = out_sig;
  OPEN_PARM_BLOCK;

  //GET_LONG_PARM(Initial_Seed);
  //Seed = Initial_Seed;
  //GET_LONG_PARM(Block_Size);

  //GET_DOUBLE_PARM(Phase_In_Deg);
  GET_DOUBLE_PARM(Beg_Freq_In_Hz);
  GET_DOUBLE_PARM(End_Freq_In_Hz);
  GET_INT_PARM(Num_Freqs);
  GET_INT_PARM(Num_Holdoff_Passes);

  MAKE_OUTPUT(Out_Sig);
  //-----------------------------------
  // controls

  Inst_Freq_Ctl = inst_freq_ctl;
  Block_Phase_Ctl = block_phase_ctl;
  Rx_Enable = rx_enable;
}
//====================================================
SweptToneTransmitter::~SweptToneTransmitter( void){};
//====================================================
void SweptToneTransmitter::Initialize(void)
{
  *DebugFile << "Now in SweptToneTransmitter::Initialize()" << endl;

  Block_Size = Out_Sig->GetBlockSize();
  Samp_Intvl = Out_Sig->GetSampIntvl();
  //Phase_Increm = Freq_In_Hz * Samp_Intvl;
  Base_Phase = 0.125;
}
//====================================================
int SweptToneTransmitter::Execute()
{
  int is;
  float *out_sig_ptr;
  double base_phase, phase_increm;
  double freq_in_hz;

   if(PassNumber < Num_Holdoff_Passes)
   {
      freq_in_hz = Beg_Freq_In_Hz;
      Rx_Enable->SetValue(false);
   }
   else if(PassNumber < Num_Holdoff_Passes + Num_Freqs)
   {
      freq_in_hz = Beg_Freq_In_Hz + 
               (PassNumber - Num_Holdoff_Passes) * (End_Freq_In_Hz - Beg_Freq_In_Hz) /(Num_Freqs-1);
      Rx_Enable->SetValue(true);
   }
   else
   {
      Rx_Enable->SetValue(false);
   }

   if(PassNumber < Num_Holdoff_Passes + Num_Freqs)
   {
      //--------------------------------
      //  Get pointer for output buffer

      out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);
      base_phase = Base_Phase;
      phase_increm = freq_in_hz * Samp_Intvl;
      Block_Phase_Ctl->SetValue(base_phase);
      Inst_Freq_Ctl->SetValue(freq_in_hz);

      for (is=0; is<Block_Size; is++)
      {
         *out_sig_ptr++ = sin(TWO_PI * (base_phase + (is*phase_increm)) );
      }
      Base_Phase = fmod(Base_Phase + Block_Size*Phase_Increm,1.0);
   }
   return(_MES_AOK);
}

