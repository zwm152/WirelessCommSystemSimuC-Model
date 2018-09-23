//
//  File = chan_diff_mp_uncorr.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include <math.h>
#include "parmfile.h"
#include "chan_diff_mp_uncorr.h"
#include "model_graph.h"
//#include "cmsg.h"
//#include "sigstuff.h"
//#include "actsysgp.h"
//#include "misdefs.h"
//#include "gensig.h"
//#include "iir_dsgn.h"
//#include "bilinear.h"
//#include "unq_iir.h"
#include "gausrand.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;

//======================================================
// normal constructor

ChanDiffuseMultipathUncorr::ChanDiffuseMultipathUncorr( char* instance_name,
                  PracSimModel* outer_model,
                  Signal<std::complex<float> >* in_sig,
                  Signal<std::complex<float> >* out_sig)
              :PracSimModel(instance_name,
                            outer_model)
{
  MODEL_NAME(ChanDiffuseMultipathUncorr);
   ostrstream *temp_stream;
   char *name_str;

  In_Sig = in_sig;
  Out_Sig = out_sig;

  OPEN_PARM_BLOCK;

  GET_LONG_PARM(Initial_Seed);
  Seed = Initial_Seed;
  GET_INT_PARM( Num_Taps);
  Multipath_Profile = new double[Num_Taps];
  GET_DOUBLE_PARM_ARRAY(Multipath_Profile, Num_Taps);

  //----------------------------------------------
  MAKE_INPUT(In_Sig);
  MAKE_OUTPUT(Out_Sig);



  //-----------------------------------------------------------
  // This must come last.  Can not create subordinate models
  // until after all setup for parent model is complete
  Gain_Filter = new ButterworthFilterByIir<float>*[Num_Taps];
  for(int i=0; i<Num_Taps; i++) {
      temp_stream = new ostrstream;
      *temp_stream << GetInstanceName() << ":Gain_Filter[" << i << "]" << ends;
      name_str = temp_stream->str();
      Gain_Filter[i] = new ButterworthFilterByIir<float>( name_str, this );
      delete temp_stream;
  }

}
//=============================================
ChanDiffuseMultipathUncorr::~ChanDiffuseMultipathUncorr( void ){ };
//===========================================
void ChanDiffuseMultipathUncorr::Initialize(void)
{
  DetailedResults << "Now in CmpxAgc::Initialize()" << endl;

  Proc_Block_Size = In_Sig->GetBlockSize();
  Samp_Intvl = In_Sig->GetSampIntvl();

  In_Buf = new std::complex<float>[Num_Taps];
  for(int i=0; i<Num_Taps; i++) {
      In_Buf[i] = 0.0;
      (Gain_Filter[i])->Initialize(Proc_Block_Size, Samp_Intvl);
  }

  Proc_Block_Size = In_Sig->GetBlockSize();
//  ebno_scaled = double( pow(10,(Desired_Eb_No/10.)) *(In_Sig->GetSampIntvl()) );

}
//=============================================
int ChanDiffuseMultipathUncorr::Execute(void)
{
  int is, tap_idx;
  float imag_rv, real_rv;
  float g_real, g_imag;
  std::complex<float> mp_sig_val, gain;
  std::complex<float> *out_sig_ptr, *in_sig_ptr;

  long seed = Seed;

  //---------------------------------------------
  // if specified, calculate the power of the input

  out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);
  in_sig_ptr = GET_INPUT_PTR(In_Sig);

  //-------------------------------------------------
  //  main loop
   for(is=0; is<Proc_Block_Size; is++) {
      for(tap_idx=Num_Taps-1; tap_idx>=1; tap_idx--) {
         In_Buf[tap_idx] = In_Buf[tap_idx-1];
      }
      In_Buf[0] = *in_sig_ptr++;

      mp_sig_val = std::complex<float>(0.0,0.0);
      for(tap_idx=0; tap_idx<Num_Taps; tap_idx++) {
         // generate two gaussian RVs
         GaussRandom(&seed, &real_rv);
         g_real = (Gain_Filter[tap_idx])->ProcessSample(real_rv);
         GaussRandom(&seed, &imag_rv);
         g_imag = (Gain_Filter[tap_idx])->ProcessSample(imag_rv);
         gain = std::complex<float>(g_real, g_imag);
         gain *= float(Multipath_Profile[tap_idx]);
         mp_sig_val += gain * In_Buf[tap_idx];
      }
      *out_sig_ptr++ = mp_sig_val;
   }// end of main loop

  // put back variables that have changed
  Seed = seed;

  //----------------------------------------------

  return(_MES_AOK);
}

