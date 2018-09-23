//
//  File = yule_walk_psd.cpp
//

#include <stdlib.h>
#include "parmfile.h"
#include "model_graph.h"
#include "yule_walk_psd.h"
#include "trianglr.h"
#include "hamming.h"
#include "hann.h"
#include "fft_T.h"
#include "dump_spect.h"
#include "autometh.h"
#include "levin.h"
#include "ar_spec.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
#define _NO_ZERO_ENDS 0
#define _ZERO_ENDS 1

extern ParmFile *ParmInput;
extern int PassNumber;

//======================================================

YuleWalkerPsdEstim::YuleWalkerPsdEstim(  
                              char* instance_name,
                              PracSimModel* outer_model,
                              Signal<float>* in_sig )
         :PracSimModel( instance_name,
                        outer_model )
{
   MODEL_NAME(YuleWalkerPsdEstim);
   OPEN_PARM_BLOCK;
   GET_INT_PARM(Seg_Len);
   GET_INT_PARM(Ar_Order);
   GET_INT_PARM(Hold_Off);
   GET_INT_PARM(Num_Freq_Pts);

   Psd_File_Name = new char[64];
   strcpy(Psd_File_Name, "\0");
   GET_STRING_PARM(Psd_File_Name);
   GET_DOUBLE_PARM(Freq_Norm_Factor);
   GET_BOOL_PARM(Output_In_Decibels);
   GET_BOOL_PARM(Plot_Two_Sided);
   GET_BOOL_PARM(Halt_When_Completed);

   In_Sig = in_sig;
   MAKE_INPUT(In_Sig);

   Time_Seg = new double[Seg_Len];
   Sample_Spectrum = new double[Seg_Len];

   for(int is=0; is<Seg_Len; is++){
      Sample_Spectrum[is] = 0.0;
   }
   Processing_Completed = false;
}
//======================================================
YuleWalkerPsdEstim::~YuleWalkerPsdEstim( void ){ };

//======================================================
void YuleWalkerPsdEstim::Initialize(void)
{
   Segs_In_Est = 0;
   Samps_Needed = Seg_Len;
   Block_Size = In_Sig->GetBlockSize();
   Samp_Intvl = In_Sig->GetSampIntvl();
};

//======================================================
int YuleWalkerPsdEstim::Execute()
{
   int is;
   int error_status;
#ifdef _DEBUG
   *DebugFile << "In YuleWalkerPsdEstim::Execute\0" 
              << endl;
#endif


   if(Processing_Completed) return(_MES_AOK);
   if(PassNumber < Hold_Off) return (_MES_AOK);
   //--------------------------------
   //  Get pointers for buffers

   float *in_sig_ptr = GET_INPUT_PTR(In_Sig);

   int samps_avail = Block_Size;

   while(Samps_Needed <= samps_avail){
      //  The new input block has enough samples to
      //  finish a segment.

      //  Fill up FFT buffer by getting Samps_Needed
      //  input samples.
      for(is=Samps_Needed; is>0; is--){
         Time_Seg[Seg_Len - is] = *in_sig_ptr++;
      }
      samps_avail -= Samps_Needed;

      AutocorrMethCorrMtx *corr_mtx = 
            new AutocorrMethCorrMtx(  
                  Time_Seg, 
                  Seg_Len, 
                  Ar_Order);

      double *correl_vec = corr_mtx->GetCol(1);
      double *a_vec = new double[Ar_Order+1];
      double driving_noise_var;

      error_status = LevinsonRecursion(   
         correl_vec,
         Ar_Order,
         a_vec,
         &driving_noise_var);

      ArSpectrum *ar_spectrum = new ArSpectrum( 
               Ar_Order,
               a_vec,
               Samp_Intvl,
               driving_noise_var, //true_ar_drv_var );
               Num_Freq_Pts,
               1.0/(2*Samp_Intvl));

      ar_spectrum->DumpSpectrum( Psd_File_Name, true);

      if(Halt_When_Completed)
      {
#ifdef _DEBUG
         *DebugFile << "Execution halted by " 
                    << GetModelName() << endl;
#endif
         exit(0);
      }
   }// end of while

   //---------------------------------------------------
   //  The number of avail new samples is not sufficient
   //  to finish a segment.  Copy the avaialble samples
   //  and then wait for the next pass to get some more.

   for(is=0; is<samps_avail; is++){
      Time_Seg[Seg_Len - Samps_Needed + is] = 
                                       *in_sig_ptr++;
   }
   Samps_Needed -= samps_avail;
   return(_MES_AOK);
}
//======================================================
