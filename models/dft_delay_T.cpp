//
//  File = cpx_dft_delay.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "dft_delay_T.h"
#include "misdefs.h"
#include "model_graph.h"
#include "sigplot.h"
#include "dit_pino_T.h"
#include "dit_nipo_T.h"
#include "complex_io.h"
extern ParmFile *ParmInput;
extern SignalPlotter SigPlot;
extern int PassNumber;
//ofstream CorrFile("corr_res.txt", ios::out);

//======================================================

template< class T >
DftDelay< T >::DftDelay( char* instance_name,
                    PracSimModel* outer_model,
                    Signal< T >* in_sig,
                    Signal< T >* out_sig)
            :PracSimModel(instance_name,
                          outer_model)
{
   ENABLE_MULTIRATE;
   In_Sig = in_sig;
   Out_Sig = out_sig;

   OPEN_PARM_BLOCK;

   GET_INT_PARM(Fft_Size);
   GET_DOUBLE_PARM(Dt_For_Fft);
   GET_FLOAT_PARM(Overlap_Save_Mem);
   GET_DOUBLE_PARM(K_Shift);

   Num_Saved_Samps = int(Overlap_Save_Mem/Dt_For_Fft + 0.5);
   Block_Size = Fft_Size - Num_Saved_Samps;

   MAKE_OUTPUT(Out_Sig);
   MAKE_INPUT(In_Sig);

   SET_SAMP_INTVL(In_Sig, Dt_For_Fft);
   SET_BLOCK_SIZE(In_Sig, Block_Size);

   SET_SAMP_INTVL(Out_Sig, Dt_For_Fft);
   SET_BLOCK_SIZE(Out_Sig, Block_Size);

   //SET_DELAY( In_Sig, Out_Sig, Group_Delay_Offset);

};
template< class T >
DftDelay< T >::~DftDelay( void )
{
   if(Full_Buffer != NULL) delete []Full_Buffer;
   if(Adj_Resp != NULL) delete []Adj_Resp;
   delete []Pt_Data_Fname;
   delete []Resid_Data_Fname;
   delete []Stretched_Data_Fname;
};

template< class T >
void DftDelay< T >::Initialize(void)
{
   int i;
   double tmp_nsexp, frac_part, int_part;
   //double k_shift = 22.25;
   //PointDataFile input_file;
   std::complex<float> exponent;
   std::complex<float> pseudo_complex;

   tmp_nsexp = log10(double(Fft_Size))/log10(2.0);
   frac_part = modf(tmp_nsexp, &int_part);

   //------------------------------------------------------------
   //  initialize derived parameters

   Ns_Exp = int_part;

   Full_Buffer = new std::complex<float>[Fft_Size];
   for(i=0; i<Fft_Size; i++)
   {
      Full_Buffer[i] = std::complex<float>(0.0,0.0);
   }

   Adj_Resp = new std::complex<float>[Fft_Size];

   //ofstream* resp_file = new ofstream("freq_resp.txt\0", ios::out);
   for(i=0; i<Fft_Size; i++)
   {
      //*Pt_Data_File >> pseudo_complex;
      Adj_Resp[i] = std::complex<float>(
                           float(cos(TWO_PI*i*K_Shift/Fft_Size)),
                           float(-sin(TWO_PI*i*K_Shift/Fft_Size)));
      Adj_Resp[i] /= double(Fft_Size);
      //*resp_file << i << ", " << time_resp[i] << ", "
      //      << std::abs(time_resp[i]) << ", " << std::arg(time_resp[i])<< endl;
   }
//exit(44);
   return;
}

template< class T >
int DftDelay< T >::Execute()
{
   int is, ns_exp, fft_size, i;
   int block_size, num_saved_samps;
   T *in_sig_ptr;
   T *out_sig_ptr;

   //-------------------------------------------------------
   //  Copy frequently accessed member vars into local vars
   block_size = Block_Size;
   num_saved_samps = Num_Saved_Samps;
   ns_exp = Ns_Exp;

   //----------------------------------------
   // Get pointers for input and output
   in_sig_ptr = GET_INPUT_PTR(In_Sig);
   out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);

   //--------------------------------------

   //memcpy(Full_Buffer, in_sig_ptr, Block_Size*sizeof(std::complex<float>));

   for(i=0; i<Block_Size; i++)
   {
      Full_Buffer[i] = std::complex<float>(in_sig_ptr[i],0.0);
   }

   if(PassNumber == 2)
   {
      ofstream *pass_in_file = new ofstream("FftInput_2.txt\0",ios::out);
      int ii;
      for(ii=Block_Size; ii<Fft_Size; ii++)
      {
         //*pass_in_file << ((PassNumber-2)*Fft_Size+ii) << ", " << Full_Buffer[ii] << endl;
         *pass_in_file << (ii - Fft_Size) << ", " << Full_Buffer[ii] << endl;
      }
      for(ii=0; ii<Fft_Size; ii++)
      {
         //*pass_in_file << ((PassNumber-1)*Fft_Size+ii) << ", " << Full_Buffer[ii] << endl;
         *pass_in_file << ii << ", " << Full_Buffer[ii] << endl;
      }
      pass_in_file->close();
      //exit(77);
   }

   // transform block of input samples
   FftDitNipo( Full_Buffer, Fft_Size);

   // multiply by sampled frequency response
   for( is=0; is<Fft_Size; is++)
   {
      Full_Buffer[is] *= Adj_Resp[is];
   }

   // transform back to time domain
   IfftDitNipo( Full_Buffer, Fft_Size);

   // copy results to output buffer
   //memcpy(out_sig_ptr, Full_Buffer, Block_Size*sizeof(std::complex<float>));
   for(i=0; i<Block_Size; i++)
   {
      out_sig_ptr[i] = Full_Buffer[i].real();
   }

   //if(PassNumber == 2)
   //{
   //   ofstream *sing_file = new ofstream("FftOutput_2.txt\0",ios::out);
   //   for(int ii=0; ii<Fft_Size; ii++)
   //   {
   //      //*sing_file << ((PassNumber-1)*46+ii) << ", " << Full_Buffer[ii] << endl;
   //      *sing_file << ii << ", " << Full_Buffer[ii] << endl;
   //   }
   //   sing_file->close();
   //   exit(77);
   //}

   if(Block_Size >= Num_Saved_Samps)
   {
      //memcpy(  &Full_Buffer[Block_Size],
      //         &in_sig_ptr[block_size-num_saved_samps],
      //         num_saved_samps*sizeof(std::complex<float>));
      for(i=0;i<num_saved_samps; i++)
      {
         Full_Buffer[Block_Size+i] = in_sig_ptr[block_size-num_saved_samps+i];
      }
   }
   else
   {
      memcpy(  &Full_Buffer[Block_Size],
               &in_sig_ptr[2*block_size],
               (num_saved_samps-block_size)*sizeof(std::complex<float>));

      memcpy(  &Full_Buffer[num_saved_samps],
               in_sig_ptr,
               block_size*sizeof(std::complex<float>));
   }
   //---------------------
   return(_MES_AOK);
}
template DftDelay< float >;
