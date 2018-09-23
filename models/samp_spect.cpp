//
//  File = samp_spect.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "model_graph.h"
#include "samp_spect.h"
#include "fft_T.h"
#include "dump_spect.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

extern ParmFile *ParmInput;
extern int PassNumber;

//======================================================

template <class T>
SampleSpectrum<T>::SampleSpectrum( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal<T>* in_sig )
                :PracSimModel( instance_name,
                                outer_model )
{
  MODEL_NAME(SampleSpectrum);
  OPEN_PARM_BLOCK;
  GET_INT_PARM(Seg_Len);
  GET_INT_PARM(Fft_Len);
  GET_INT_PARM(Hold_Off);

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
  Sample_Spectrum = new double[Fft_Len];
  Freq_Seg = new std::complex<double>[Fft_Len];

  Psd_File = new ofstream(Psd_File_Name, ios::out);
  Processing_Completed = false;


}
template <class T>
SampleSpectrum<T>::~SampleSpectrum( void ){ };

template <class T>
void SampleSpectrum<T>::Initialize(void)
{
  Segs_In_Est = 0;
  Samps_Needed = Seg_Len;
  Block_Size = In_Sig->GetBlockSize();
  Samp_Intvl = In_Sig->GetSampIntvl();
  Delta_F = 1.0/(Samp_Intvl*Fft_Len);

};

template <class T>
int SampleSpectrum<T>::Execute()
{
   int i,is;
   #ifdef _DEBUG
      *DebugFile << "In SampleSpectrum::Execute\0" << endl;
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

      //  Perform FFT
      FFT<double>(   Time_Seg,
                     Freq_Seg,
                     Seg_Len,
                     Fft_Len);

      for(i=0; i<Seg_Len; i++)
      {
         Sample_Spectrum[i] = Samp_Intvl * std::norm(Freq_Seg[i])/Seg_Len;
      }

      for(is=0; is<Seg_Len; is++)
      {
         Time_Seg[is] = 0.0;
      }
      Samps_Needed = Seg_Len;

//      Segs_In_Est++;

      // is it time to dump the results?
//      if(Segs_In_Est == Num_Segs_To_Avg)
//      {
         DumpSpectrum(  Sample_Spectrum,
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
template SampleSpectrum<float>;
