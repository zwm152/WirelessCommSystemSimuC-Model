//
//  File = mskmod.h
//

#ifndef _MSKMOD_H_
#define _MSKMOD_H_

#include "signal_T.h"

//======================================================
class MskModulator : public PracSimModel
{
public:
   MskModulator(  char* instance_name,
                  PracSimModel *outer_model,
                  Signal< float >* in_signal_i,
                  Signal< float >* in_signal_q,
                  Signal< complex<float> >* out_signal,
                  Signal< float >* mag_signal,
                  Signal< float >* phase_signal );

   ~MskModulator(void);
   void Initialize(void);
   int Execute(void);

private:
   float Phase_Unbal;
   float Amp_Unbal;
   double Pi_Over_Bit_Dur;
   double Bit_Durat;
   double Samp_Intvl;
   float Subcar_Misalign;
   float Data_Skew;
   int Shaping_Is_Bipolar;
   std::complex<float> Phase_Shift;
   int Samps_Out_Cnt;
   int Block_Size;
   Signal< float > *I_In_Sig;
   Signal< float > *Q_In_Sig;
   Signal< std::complex<float> > *Cmpx_Out_Sig;  
   Signal< float > *Mag_Out_Sig;
   Signal< float > *Phase_Out_Sig;
};

#endif
