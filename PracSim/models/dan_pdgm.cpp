//
//  File = dan_pdgm.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "model_graph.h"
#include "dan_pdgm.h"
#include "fft_T.h"
#include "dump_spect.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

extern ParmFile *ParmInput;
extern int PassNumber;

//======================================================

template <class T>
DaniellPeriodogram<T>::DaniellPeriodogram( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal<T>* in_sig )
                :PracSimModel( instance_name,
                                outer_model )
{
  MODEL_NAME(DaniellPeriodogram);
  OPEN_PARM_BLOCK;
  GET_INT_PARM(Seg_Len);
  GET_INT_PARM(Fft_Len);
  GET_INT_PARM(Hold_Off);
  GET_INT_PARM(Big_P);

  Psd_File_Name = new char[64];
  strcpy(Psd_File_Name, "\0");
  GET_STRING_PARM(Psd_File_Name);
  GET_DOUBLE_PARM(Freq_Norm_Factor);
  GET_BOOL_PARM(Output_In_Decibels);
  GET_BOOL_PARM(Plot_Two_Sided);
  GET_BOOL_PARM(Halt_When_Completed);

  In_Sig = in_sig;
  MAKE_INPUT(In_Sig);


  Time_Seg = new T[Seg_Len];
  Dan_Pdgm = new double[Fft_Len];
  Freq_Seg = new std::complex<double>[Fft_Len];

  Psd_File = new ofstream(Psd_File_Name, ios::out);
  Processing_Completed = false;


}
template <class T>
DaniellPeriodogram<T>::~DaniellPeriodogram( void ){ };

template <class T>
void DaniellPeriodogram<T>::Initialize(void)
{
  Samps_Needed = Seg_Len;
  Block_Size = In_Sig->GetBlockSize();
  Samp_Intvl = In_Sig->GetSampIntvl();
  Delta_F = 1.0/(Samp_Intvl*Fft_Len);

};

template <class T>
int DaniellPeriodogram<T>::Execute()
{
   int k, m,is;
   double scale_factor, sum;
   #ifdef _DEBUG
      *DebugFile << "In DaniellPeriodogram::Execute\0" << endl;
   #endif


   if(Processing_Completed) return(_MES_AOK);
   if(PassNumber < Hold_Off+1) return (_MES_AOK);
   //--------------------------------
   //  Get pointers for buffers

   T *in_sig_ptr = GET_INPUT_PTR(In_Sig);

   int samps_avail = Block_Size;

   while(Samps_Needed <= samps_avail)
   {
      //  The new input block has enough samples to finish a segment.

      //  Fill up FFT buffer by getting Samps_Needed input samples.
      for(is=Samps_Needed; is>0; is--)
      {
         Time_Seg[Seg_Len - is] = *in_sig_ptr++;
      }
      samps_avail -= Samps_Needed;

      scale_factor = Seg_Len*(2*Big_P+1);
      //  Perform FFT
      FFT<double>(   Time_Seg,
                     Freq_Seg,
                     Seg_Len,
                     Fft_Len);

      //--------------------------------------
      // Compute firts P points of periodogram

      for(m=0; m<Big_P; m++)
      {
         sum = 0.0;
         for( k=m-Big_P; k<0; k++)
         {
            sum += std::norm(Freq_Seg[Fft_Len + k]);
         }
         for(k=0; k<=(m+Big_P); k++)
         {
            sum += std::norm(Freq_Seg[k]);
         }
         Dan_Pdgm[m] = Samp_Intvl*sum/scale_factor;
      }
      //-------------------------------------------------------
      //  Compute periodogram points P thru (N/2)-1

      for(m=Big_P; m<(Fft_Len/2); m++)
      {
         sum = 0.0;
         for( k=m-Big_P; k<=m+Big_P; k++)
         {
            sum += std::norm(Freq_Seg[k]);
         }
         Dan_Pdgm[m] = Samp_Intvl*sum/scale_factor;
      }


      // is it time to dump the results?
//      if(Segs_In_Est == Num_Segs_To_Avg)
//      {
         DumpSpectrum(  Dan_Pdgm,
                        Fft_Len,
                        Delta_F,
                        Freq_Norm_Factor,
                        Output_In_Decibels,
                        Plot_Two_Sided,
                        Psd_File);

         Processing_Completed = true;
         Psd_File->close();
         if(Halt_When_Completed)
         {
            #ifdef _DEBUG
               *DebugFile << "Execution halted by " << GetModelName() << endl;
            #endif
            exit(0);
         }
//      }

   }// end of while
  //  The number of avail new samples is not sufficient to finish a segment.
  //  Copy the avaialble samples and then wait for the next pass
  //  to get some more.

   for(is=0; is<samps_avail; is++)
   {
      Time_Seg[Seg_Len - Samps_Needed + is] = *in_sig_ptr++;
   }
   Samps_Needed -= samps_avail;

   return(_MES_AOK);

}
template DaniellPeriodogram<float>;
