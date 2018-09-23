//
//  File = bart_pdgm_wind.cpp
//

#include <stdlib.h>
#include "parmfile.h"
#include "model_graph.h"
#include "bart_pdgm_wind.h"
#include "trianglr.h"
#include "hamming.h"
#include "hann.h"
#include "fft_T.h"
#include "dump_spect.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
#define _NO_ZERO_ENDS 0
#define _ZERO_ENDS 1

extern ParmFile *ParmInput;
extern int PassNumber;

//======================================================
template <class T>
BartlettPeriodogramWindowed<T>::
            BartlettPeriodogramWindowed( 
                              char* instance_name,
                              PracSimModel* outer_model,
                              Signal<T>* in_sig )
                :PracSimModel( instance_name,
                               outer_model )
{
   int is;
   MODEL_NAME(BartlettPeriodogram);
   OPEN_PARM_BLOCK;
   GET_INT_PARM(Seg_Len);
   GET_INT_PARM(Fft_Len);
   GET_INT_PARM(Hold_Off);
   GET_INT_PARM(Num_Segs_To_Avg);
   GET_DOUBLE_PARM(Freq_Norm_Factor);
   GET_BOOL_PARM(Output_In_Decibels);
   GET_BOOL_PARM(Plot_Two_Sided);
   GET_BOOL_PARM(Halt_When_Completed);
   GET_BOOL_PARM(Using_Window);
   Psd_File_Name = new char[64];
   strcpy(Psd_File_Name, "\0");
   GET_STRING_PARM(Psd_File_Name);

   if(Using_Window){
      Window_Shape = 
         GetWindowShapeParm("Window_Shape\0");

      switch(Window_Shape){
      case WINDOW_SHAPE_TRIANGULAR:
         Data_Window = new TriangularWindow( Seg_Len,
                                       _NO_ZERO_ENDS );
         break;
      case WINDOW_SHAPE_HAMMING:
         Data_Window = new HammingWindow( Seg_Len );
         break;
      case WINDOW_SHAPE_HANN:
         Data_Window = new HannWindow( Seg_Len, 
                                       _NO_ZERO_ENDS );
         break;
      }

      Window_Taps = Data_Window->GetDataWindow();
      Window_Power = 0.0;
      for(is=0; is<Seg_Len; is++){
         Window_Power +=Window_Taps[is]*Window_Taps[is];
      }
      Window_Power /= double(Seg_Len);
      double window_scale = sqrt(Window_Power);
      for(is=0; is<Seg_Len; is++){
         Window_Taps[is] /= window_scale;
      }
   }

   In_Sig = in_sig;
   MAKE_INPUT(In_Sig);

   Time_Seg = new T[Seg_Len];
   Win_Time_Seg = new T[Seg_Len];
   Sample_Spectrum = new double[Fft_Len];
   Freq_Seg = new std::complex<double>[Fft_Len];

   for(is=0; is<Fft_Len; is++){
      Sample_Spectrum[is] = 0.0;
   }

   Psd_File = new ofstream(Psd_File_Name, ios::out);
   Processing_Completed = false;
}
//======================================================
template <class T>
BartlettPeriodogramWindowed<T>::~BartlettPeriodogramWindowed( void ){ };

//======================================================
template <class T>
void BartlettPeriodogramWindowed<T>::Initialize(void)
{
   Segs_In_Est = 0;
   Samps_Needed = Seg_Len;
   Block_Size = In_Sig->GetBlockSize();
   Samp_Intvl = In_Sig->GetSampIntvl();
   Delta_F = 1.0/(Samp_Intvl*Fft_Len);

};
//======================================================
template <class T>
int BartlettPeriodogramWindowed<T>::Execute()
{
   int i,is;
#ifdef _DEBUG
   *DebugFile 
      << "In BartlettPeriodogramWindowed::Execute\0" 
      << endl;
#endif

   if(Processing_Completed) return(_MES_AOK);
   if(PassNumber < Hold_Off) return (_MES_AOK);
   //--------------------------------
   //  Get pointers for buffers

   T *in_sig_ptr = GET_INPUT_PTR(In_Sig);

   int samps_avail = Block_Size;

   while(Samps_Needed <= samps_avail)
   {
      //  The new input block has enough samples
      //  to finish a segment.
      //  Fill up FFT buffer by getting 
      //  Samps_Needed input samples.
      for(is=Samps_Needed; is>0; is--){
         Time_Seg[Seg_Len - is] = *in_sig_ptr++;
      }
      samps_avail -= Samps_Needed;

      if(Using_Window){
         for(is=0; is<Seg_Len; is++){
            Win_Time_Seg[is] = 
                  Window_Taps[is]*Time_Seg[is];
         }
      }
      else{
         for(is=0; is<Seg_Len; is++){
            Win_Time_Seg[is] = Time_Seg[is];
         }
      }

      //  Perform FFT
      FFT<double>(   Win_Time_Seg,
                     Freq_Seg,
                     Seg_Len,
                     Fft_Len);

      for(i=0; i<Seg_Len; i++){
         Sample_Spectrum[i] += Samp_Intvl * 
                  std::norm(Freq_Seg[i])/Seg_Len;
      }

      for(is=0; is<Seg_Len; is++){
         Time_Seg[is] = 0.0;
      }
      Samps_Needed = Seg_Len;
      Segs_In_Est++;

      // is it time to dump the results?
      if(Segs_In_Est == Num_Segs_To_Avg){
         for(i=0; i<Seg_Len; i++){
            Sample_Spectrum[i] /= 
                        double(Num_Segs_To_Avg);
         }
         DumpSpectrum(  Sample_Spectrum,
                        Fft_Len,
                        Delta_F,
                        Freq_Norm_Factor,
                        Output_In_Decibels,
                        Plot_Two_Sided,
                        Psd_File);

         Processing_Completed = true;
         Psd_File->close();

         if(Halt_When_Completed){
#ifdef _DEBUG
            *DebugFile << "Execution halted by " 
                       << GetModelName() << endl;
#endif
            exit(0);
         }
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
template BartlettPeriodogramWindowed<float>;
