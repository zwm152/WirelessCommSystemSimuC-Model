//
//  File = correlator.cpp
//

#include <stdlib.h>
#include <complex>
//#include <fstream>
#include "parmfile.h"
#include "correlator.h"
#include "misdefs.h"
#include "model_graph.h"
#include "sigplot.h"
#include "dit_pino_T.h"
#include "dit_nipo_T.h"
#include "dft_T.h"
#include "unwrap.h"
#include "complex_io.h"
extern ParmFile* ParmInput;
//extern SignalPlotter SigPlot;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
//ofstream CorrFile("corr_res.txt", ios::out);

//======================================================

RealCorrelator::RealCorrelator( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<float>* in_sig,
                                Signal<float>* ref_sig,
                                Signal<float>* out_sig,
                                Control<float>* delay_at_max_corr,
                                Control<int>* samps_delay_at_max_corr)
            :PracSimModel(instance_name,
                          outer_model)
{
  In_Sig = in_sig;
  Ref_Sig = ref_sig;
  Out_Sig = out_sig;

  Delay_At_Max_Corr = delay_at_max_corr;
  Samps_Delay_At_Max_Corr = samps_delay_at_max_corr;
  //Max_Corr_Out = max_corr_out;

  OPEN_PARM_BLOCK;

  GET_INT_PARM(Num_Corr_Passes);
  GET_BOOL_PARM(Limited_Search_Window_Enab);
  if(Limited_Search_Window_Enab)
  {
     GET_INT_PARM(Search_Window_Beg);
     GET_INT_PARM(Search_Window_End);
  }
  GET_BOOL_PARM(Invert_Input_Sig_Enab);
  //GET_INT_PARM(Smoothing_Sidelobe_Len);

  MAKE_OUTPUT(Out_Sig);
  MAKE_INPUT(In_Sig);
  MAKE_INPUT(Ref_Sig);

  SAME_RATE(In_Sig, Out_Sig);
  SAME_RATE(Ref_Sig, Out_Sig);

  //control output: Delay_At_Max_Corr
  //control output: Max_Corr_Angle_Out
  Spectrum_File = new ofstream("spectrum.txt\0", ios::out);
}
RealCorrelator::~RealCorrelator( void ){ };

void RealCorrelator::Initialize(void)
{
   int dummy_size, i;
   double tmp_nsexp;
   double frac_part, int_part;
   float sample=0;

   Proc_Block_Size = Out_Sig->GetBlockSize();
   Samp_Intvl = Out_Sig->GetSampIntvl();

   In_Sig_Buf = new AuxSignalBuffer<float>(sample, Proc_Block_Size);
   Ref_Sig_Buf = new AuxSignalBuffer<float>(sample, Proc_Block_Size);

   tmp_nsexp = log10(double(Proc_Block_Size))/log10(double(2));
   frac_part = modf(tmp_nsexp, &int_part);

   if( frac_part != 0.0 )
      {
      Ns_Exp = int_part + 2;
      }
   else
      {
      Ns_Exp = int_part + 1;
      }

   Full_Corr_Size = pow(double(2), Ns_Exp);
   X = new std::complex<float>[Full_Corr_Size];
   Y = new std::complex<float>[Full_Corr_Size];

   if(Limited_Search_Window_Enab)
      {
      if(Search_Window_Beg < 0)
         {
         Neg_Window_Beg = Full_Corr_Size + Search_Window_Beg;
         Pos_Window_Beg = 0;
         }
      else
         {
         Neg_Window_Beg = Full_Corr_Size;
         Pos_Window_Beg = Search_Window_Beg;
         }
      if(Search_Window_End >= 0)
         {
         Pos_Window_End = Search_Window_End;
         Neg_Window_End = Full_Corr_Size-1;
         }
      else
         {
         Pos_Window_End = -1;
         Neg_Window_End = Search_Window_End;
         }
      }
   else
      {
      Neg_Window_Beg = Full_Corr_Size/2 + 1;
      Neg_Window_End = Full_Corr_Size-1;
      Pos_Window_Beg = 0;
      Pos_Window_End = Full_Corr_Size/2 - 1;
      }


   dummy_size = Full_Corr_Size - Proc_Block_Size;
   Zero_Array = new std::complex<float>[dummy_size];
   for( i = 0; i < dummy_size; i++)
   {
      Zero_Array[i] = std::complex<float>(0.0, 0.0);
   }
   Size_Of_FComplex = sizeof(std::complex<float>);

   Max_Corr = 0.0;
   Max_Corr_Time = 0.0;

  Diff_Response = new std::complex<float>[Full_Corr_Size];
  for( i=0; i<Full_Corr_Size; i++)
  {
    Diff_Response[i] = 0.0;
  }
   Corr_Pass_Count = 0;
   //INITIALIZATION_REPORT(BasicResults);
   return;
}

int RealCorrelator::Execute()
{
   using std::complex;
   float *in_sig_ptr, *in_sig_buf_ptr;
   float *ref_sig_ptr, *ref_sig_buf_ptr;
   int in_sig_buf_count;
   int ref_sig_buf_count;
   float *out_sig_ptr;
   float max_corr;
   float out_mag, x_temp; // out_angle;
   float max_corr_time;
   int zero_size;
   int samp, max_samp;
   double deg_per_rad = 180.0/PI;
   double phase_deg;

   if(Corr_Pass_Count > Num_Corr_Passes) return(_MES_AOK);

   //-------------------------------------------------------
   //  Copy frequently accessed member vars into local vars
   std::complex<float> *x = X;
   std::complex<float> *y = Y;
   int proc_block_size;
   int in_sig_block_size;
   int ref_sig_block_size;
   int full_corr_size = Full_Corr_Size;
   int size_of_fcomplex = Size_Of_FComplex;
   int ns_exp = int(Ns_Exp);
   //int smoothing_sidelobe_len = Smoothing_Sidelobe_Len;
   double samp_intvl = Samp_Intvl;
   double phase_to_time;
   double phase_rad;
   double tau, tau_sum, tau_avg;
   double denom, numer;
   int regression_start, regression_stop;
   double phase_sum;
   double phase_avg;
   double idx_sum, idx_avg;
   std::complex<float> x_of_f[4096];
   bool phase_has_wrapped;
    float amp;

   //----------------------------------------
   // Get pointers for input and output
   in_sig_ptr = GET_INPUT_PTR(In_Sig);
   ref_sig_ptr = GET_INPUT_PTR(Ref_Sig);
   out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);
   proc_block_size = Proc_Block_Size;
   in_sig_block_size = In_Sig->GetValidBlockSize();
   ref_sig_block_size = Ref_Sig->GetValidBlockSize();

   //------------------------------------------------
   //  Check to see if there enough samples of input
   //  signal and reference signal to perform correlation
   //  If not, set valid block size for output to zero
   //  and then exit from this model.

   in_sig_buf_ptr = In_Sig_Buf->Load(  in_sig_ptr, 
                                       in_sig_block_size, 
                                       &in_sig_buf_count);

   ref_sig_buf_ptr = Ref_Sig_Buf->Load(ref_sig_ptr, 
                                       ref_sig_block_size,
                                       &ref_sig_buf_count);

   if( (in_sig_buf_count < proc_block_size)
      || (ref_sig_buf_count < proc_block_size))
   {
      Out_Sig->SetValidBlockSize(0);
      return(_MES_AOK);
   }
   

   //----------------------------------------
   zero_size = full_corr_size - proc_block_size;
   phase_to_time = full_corr_size * samp_intvl/TWO_PI;
   //phase_to_time = full_corr_size * samp_intvl;

  // set up arrays for FFT's
  // zero-pad from Proc_Block_Size to Full_Corr_Size
   int i;
//  memcpy( x, in_sig_ptr, proc_block_size*size_of_fcomplex);
   for( i=0; i<proc_block_size; i++)
   {
      x[i] = std::complex<float>( *in_sig_buf_ptr++, 0.0 );
   }
   In_Sig_Buf->Release(proc_block_size);

   for( i=proc_block_size; i<full_corr_size; i++)
   {
      x[i] = 0.0;
   }

   if(Invert_Input_Sig_Enab)
   {
      for( i=0; i<proc_block_size; i++)
      {
         x[i] = -x[i];
      }
   }

//  memcpy( y, ref_sig_ptr, proc_block_size*size_of_fcomplex);
   for( i=0; i<proc_block_size; i++)
   {
      y[i] = std::complex<float>( *ref_sig_buf_ptr++, 0.0 );
   }
   Ref_Sig_Buf->Release(proc_block_size);

   for( i=proc_block_size; i<full_corr_size; i++)
   {
      y[i] = 0.0;
   }

   //memset( &x[proc_block_size], 0,
   //         zero_size*sizeof(std::complex<float>));
   //memcpy( &x[proc_block_size], Zero_Array,
   //         zero_size*size_of_fcomplex);
   //memcpy( &y[proc_block_size], Zero_Array,
   //         zero_size*size_of_fcomplex);

   // perform FFT's
   //FFT(x, ns_exp, full_corr_size, ns_exp);
   //FFT(y, ns_exp, full_corr_size, ns_exp);
//   dft( x, x_of_f, full_corr_size);

   FftDitNipo<float>(x, full_corr_size);
   FftDitNipo<float>(y, full_corr_size);

//   if(PassNumber == Num_Corr_Passes)
//   {
//      ofstream  *special_file = new ofstream("special.txt\0", ios::out);
//     for(samp=0; samp < full_corr_size; samp++)
//     {
//        *special_file << samp << ", " << x[samp] << ", " << x_of_f[samp] << endl;
        //x[samp] /= float(full_corr_size);
        //x[samp] *= std::conj<float>( y[samp] ) / float(full_corr_size);
//     }
//     special_file->close();
//   }



   // perform correlation
   for(samp=0; samp < full_corr_size; samp++)
   {
      //x[samp] /= float(full_corr_size);
      x[samp] *= std::conj<float>( y[samp] ) / float(full_corr_size);
   }


   //----------------------------------------------------------
   if( (PassNumber >= 2)
      && (Corr_Pass_Count <= Num_Corr_Passes) )
   {
     tau_sum = 0.0;
     tau_avg = 0.0;

      phase_has_wrapped = false;
      for(samp=0; samp < full_corr_size; samp++)
      {
         //phase_deg = deg_per_rad * std::arg<float>(x[samp]);
         Diff_Response[samp] += x[samp];

         if(Corr_Pass_Count == Num_Corr_Passes)
         {
            //UnwrapPhase(samp-1,&phase_deg);
            amp = std::abs<float>(Diff_Response[samp]);
            phase_deg = deg_per_rad * std::arg<float>(Diff_Response[samp]);
            if(phase_deg > 100) phase_has_wrapped = true;
            if( !phase_has_wrapped) regression_stop = samp;

            //phase_deg = Phase_Response[samp]/(Num_Corr_Passes-1);
            phase_rad = PI * phase_deg/180.0;
            tau = 0.0;
            if(samp !=0)
            {
               tau = -phase_to_time * phase_rad/samp;
            }
            //if(samp > 20)
            //{
            //  tau_sum += tau;
            //  tau_avg = tau_sum/(samp-20);
            // }
            (*Spectrum_File) << samp << ", " << amp << ", " 
                            << phase_deg << ", "
                            << tau << endl;
         }

      }
      if(Corr_Pass_Count == Num_Corr_Passes)
      {
         regression_start = 5;
         //regression_stop = 74;
         if(regression_stop > 300) regression_stop = 300;
         phase_sum = 0.0;
         idx_sum = 0.0;
         for(samp=regression_start; samp < regression_stop; samp++)
         {
            phase_rad = std::arg<float>(Diff_Response[samp]);
            //phase_deg = deg_per_rad * std::arg<float>(Diff_Response[samp]);
            //phase_rad = PI * phase_deg/180.0;
            phase_sum += phase_rad;
            idx_sum += samp;
         }
         numer = 0.0;
         denom = 0.0;
         idx_avg = idx_sum / (regression_stop - regression_start);
         phase_avg = phase_sum / (regression_stop - regression_start);
         for(samp=regression_start; samp < regression_stop; samp++)
         {
            phase_rad = std::arg<float>(Diff_Response[samp]);
            numer += (samp-idx_avg)*(phase_rad - phase_avg);
            denom += (samp-idx_avg)*(samp-idx_avg);
         }
         tau = -phase_to_time * numer / denom;
         Spectrum_File->close();
         exit(-9);
      }
      Corr_Pass_Count++;
   }
   //----------------------------------------------------



   //InverseFFT(x, ns_exp, full_corr_size, ns_exp);
   //dft( x, x_of_f, full_corr_size);
   IfftDitPino<float>(x, full_corr_size);

//   if(PassNumber == Num_Corr_Passes)
//   {
//      ofstream  *special_file = new ofstream("special.txt\0", ios::out);
//     for(samp=0; samp < full_corr_size; samp++)
//     {
//        *special_file << samp << ", " << x[samp] << ", " << x_of_f[samp] << endl;
        //x[samp] /= float(full_corr_size);
        //x[samp] *= std::conj<float>( y[samp] ) / float(full_corr_size);
//     }
//     special_file->close();
//   }

   // fill the output buffer
   //memcpy( out_sig_ptr, x, proc_block_size*size_of_fcomplex);
   for(int ii=0; ii<proc_block_size; ii++)
   {
      out_sig_ptr[ii] = x[ii].real();
   }

   // Determine maximum correlation plus corresponding time
   out_mag = 0.0;
   for(samp=Neg_Window_Beg; samp <= Neg_Window_End; samp++)
   {
      //x_temp = real(x[samp]*conj<float>(x[samp]));
      x_temp = x[samp].real();
      (*DebugFile) << samp << ", " << x_temp << endl;
      if( x_temp > out_mag)
      {
         out_mag = x_temp;
         max_corr = x[samp].real();
         max_samp = samp;
      }
   }
   for(samp=Pos_Window_Beg; samp <= Pos_Window_End; samp++)
   {
      //x_temp = real(x[samp]*conj<float>(x[samp]));
      x_temp = x[samp].real();
      (*DebugFile) << samp << ", " << x_temp << endl;
      if( x_temp > out_mag)
      {
         out_mag = x_temp;
         max_corr = x[samp].real();
         max_samp = samp;
      }
   }
   (*DebugFile) << "max_samp = " << max_samp << endl;
   if( max_samp > proc_block_size ) max_samp -= full_corr_size;
   max_corr_time = max_samp*samp_intvl;
//  if( (fabs(max_corr) < 1.0e-30) )
//      {
//      max_corr = 1.0;
//      }
   Delay_At_Max_Corr->SetValue(max_corr_time);

   #ifdef _DEBUG
      (*DebugFile) << "Correlator found delay as " << max_corr_time << endl;
   #endif

   Samps_Delay_At_Max_Corr->SetValue(max_samp);

   //---------------------
   return(_MES_AOK);
}

