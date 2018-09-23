//
//  File =spec_analyzer.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "model_graph.h"
#include "spec_analyzer.h"
#include "hann.h"
#include "misdefs.h"
#include "fft_T.h"
#include "samp_spect_util.h"
#include "bart_pdgm_util.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

extern ParmFile *ParmInput;
extern int PassNumber;
extern char *DateString;
extern bool DateInFileNames;

//======================================================

template <class T>
SpectrumAnalyzer<T>::SpectrumAnalyzer( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal<T>* in_sig )
                :PracSimModel( instance_name,
                                outer_model )
{
  MODEL_NAME(SpectrumAnalyzer_T);
  OPEN_PARM_BLOCK;
  Kind_Of_Spec_Estim = GetKindOfSpectCalcParm("Kind_Of_Spec_Estim\0");
  GET_INT_PARM(Num_Segs_To_Avg);
  GET_INT_PARM(Seg_Len);
  GET_INT_PARM(Fft_Len);
  GET_INT_PARM(Hold_Off);

  //Psd_File_Name = new char[255];

  //strcpy(Psd_File_Name, "\0");
  GET_STRING_PARM(Psd_File_Name);
  //char* temp_str = new char[255];
  //strcpy(temp_str,Psd_File_Name);
  //delete[] Psd_File_Name;
  //Psd_File_Name = new char[255];



   if(DateInFileNames){
      strcat(Psd_File_Name,"_");
      strcat(Psd_File_Name, DateString);
      }
   strcat(Psd_File_Name,".txt");

  GET_DOUBLE_PARM(Norm_Factor);
  GET_DOUBLE_PARM(Freq_Norm_Factor);
  GET_BOOL_PARM(Output_In_Decibels);
  GET_BOOL_PARM(Plot_Two_Sided);
  GET_BOOL_PARM(Halt_When_Completed);
  GET_BOOL_PARM(Plot_Relative_To_Peak);

  In_Sig = in_sig;
  MAKE_INPUT(In_Sig);


  Time_Seg = new T[Seg_Len];
  Psd_Est = new double[Fft_Len];
  //Sample_Spectrum = new std::complex<double>[Fft_Len];
  for(int is=0; is<Fft_Len; is++)
    {
    Psd_Est[is] = 0.0;
    }

  Psd_File = new ofstream(Psd_File_Name, ios::out);
  Processing_Completed = false;


}
template <class T>
SpectrumAnalyzer<T>::~SpectrumAnalyzer( void ){ };

template <class T>
void SpectrumAnalyzer<T>::Initialize(void)
{
  Segs_In_Est = 0;
  Samps_Needed = Seg_Len;
  Block_Size = In_Sig->GetBlockSize();
  Samp_Intvl = In_Sig->GetSampIntvl();
  Delta_F = 1.0/(Samp_Intvl*Fft_Len);

   switch(Kind_Of_Spec_Estim)
   {
   case SPECT_CALC_SAMPLE_SPECTRUM:
      Spec_Estim = new SampleSpectrum<T>( Seg_Len,
                                             Fft_Len,
                                             Samp_Intvl);
      break;
   case SPECT_CALC_BARTLETT_PDGM:
      Spec_Estim = new BartlettPeriodogram<T>( Seg_Len,
                                             Fft_Len,
                                             Samp_Intvl);
      Data_Window = new HannWindow( Seg_Len, _NO_ZERO_ENDS );
      break;
   }

};

template <class T>
int SpectrumAnalyzer<T>::Execute()
{
   int is;
   double scale_factor;
   double *data_window;
   double total_power;
   double offset;
   #ifdef _DEBUG
      *DebugFile << "In SpectrumAnalyzer::Execute\0" << endl;
   #endif


   if(Processing_Completed) return(_MES_AOK);
   if(PassNumber < Hold_Off) return (_MES_AOK);
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

      //  Apply window to the time segment

      data_window = Data_Window->GetDataWindow();
      for(is=0; is<Seg_Len; is++) {
         Time_Seg[is] *= float(*data_window++);
      }

      //  Perform FFT
      //FFT<double>(   Time_Seg,
      //               Sample_Spectrum,
      //               Seg_Len,
      //               Fft_Len);

      //  Perform FFT
      Spec_Estim->Calculate( Time_Seg );
      Spec_Estim->GetEstimate( Psd_Est);

      for(is=0; is<Seg_Len; is++)
      {
         Time_Seg[is] = 0.0;
      }
      Samps_Needed = Seg_Len;

      //  merge sample spectrum into PSD estimate
//      for(is=0; is<Fft_Len; is++)
//      {
         //Psd_Est[is] += std::norm(Sample_Spectrum[is]);
//         Psd_Est[is] += Sample_Spectrum[is];
//      }
      Segs_In_Est++;

      // is it time to dump the results?
      if(Segs_In_Est == Num_Segs_To_Avg)
      {
         //------------------------------------
         // find peak

         double peak_val=0.0;
         int peak_idx=0;
         total_power = 0.0;
         for(is=0; is<Fft_Len; is++) {
            total_power += Psd_Est[is];
         }
         total_power /= double(Num_Segs_To_Avg);
         for(is=0; is<Fft_Len/2; is++)
         {
            if(Psd_Est[is] <= peak_val) continue;
            //else
            peak_val = Psd_Est[is];
            peak_idx = is;
         }

         //scale_factor = double(Num_Segs_To_Avg*Seg_Len*Norm_Factor);
////         scale_factor = double(Seg_Len*Norm_Factor/2.0);
//030726         scale_factor = Num_Segs_To_Avg;
            scale_factor = Num_Segs_To_Avg * Delta_F;
         //scale_factor = double(Seg_Len/2);
         if(Output_In_Decibels)
         {
            if(Plot_Relative_To_Peak)
            {
               offset = 10.0*log10(Psd_Est[peak_idx]/scale_factor);
            }
            else
            {
               offset = 10.0*log10(1.0/scale_factor);
            }

            if(Plot_Two_Sided)
            {
               for(is=-(Fft_Len/2-1); is<0; is++)
               {
                  if( Psd_Est[-is] >0 )
                  {
                     if(Plot_Relative_To_Peak)
                     {
                        (*Psd_File) << is * Delta_F * Freq_Norm_Factor << ", " 
                                    << (10.0*log10(Psd_Est[-is]/scale_factor)-offset) << endl;
                     }
                     else
                     {
                        (*Psd_File) << is * Delta_F * Freq_Norm_Factor << ", " 
                                    << (10.0*log10(Psd_Est[-is]/scale_factor)) << endl;
                     }
                  }
                  else
                  {
                     (*Psd_File) << is * Delta_F * Freq_Norm_Factor << ", -200.0" << endl;
                  }
               }
            }

            for(is=0; is<Fft_Len/2; is++)
            {
               if( Psd_Est[is] >0 )
               {
                  if(Plot_Relative_To_Peak)
                  {
                     (*Psd_File) << is * Delta_F * Freq_Norm_Factor << ", " 
                                 << (10.0*log10(Psd_Est[is]/scale_factor)-offset) << endl;
                  }
                  else
                  {
                     (*Psd_File) << is * Delta_F * Freq_Norm_Factor << ", " 
                                 << (10.0*log10(Psd_Est[is]/scale_factor)) << endl;
                  }
                  Psd_Est[is] = 0.0;
               }
               else
               {
                  (*Psd_File) << is * Delta_F * Freq_Norm_Factor << ", -200.0" << endl;
               }
            }
         }
         else
         {
            // plot as linear ordinate
            if(Plot_Two_Sided)
            {
               for(is=-(Fft_Len/2-1); is<0; is++)
               {
                  (*Psd_File) << is * Delta_F * Freq_Norm_Factor << ", " 
                              << Psd_Est[-is]/scale_factor << endl;
               }
            }

            for(is=0; is<Fft_Len/2; is++)
            {
               (*Psd_File) << is * Delta_F * Freq_Norm_Factor << ", " 
                           << Psd_Est[is]/scale_factor << endl;
               Psd_Est[is] = 0.0;
            }
         }
         Processing_Completed = true;
         Psd_File->close();
         BasicResults << Instance_Name << ": total_power = " << total_power << endl;
         if(Halt_When_Completed)
         {
            #ifdef _DEBUG
               *DebugFile << "Execution halted by " << GetModelName() << endl;
            #endif
            exit(0);
         }
      }

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
template SpectrumAnalyzer<std::complex<float> >;
template SpectrumAnalyzer<float>;
