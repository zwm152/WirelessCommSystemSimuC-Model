//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = anlg_filt_iir.cpp
//
//  The class AnalogFilterByInteg serves as the base class
//  for all the "traditional" analog filter types
//  e.g. Butterworth, Chebyshev, etc. that are modeled 
//  using an IIR filter
//

#include <math.h>
#include "misdefs.h"
#include "parmfile.h"
#include "model_graph.h"
#include "anlg_filt_iir.h"
#include "bilin_transf.h"
#include "iir_comp_resp.h"

extern ParmFile *ParmInput;

extern ofstream *DebugFile;
using std::complex;



//======================================================
//  constructor

template <class T>
AnalogFilterByIir<T>::
         AnalogFilterByIir(  char *instance_name,
                             PracSimModel *outer_model,
                             Signal<T> *in_sig,
                             Signal<T> *out_sig )
                    :PracSimModel(  instance_name,
                                    outer_model )
{
   MODEL_NAME(AnalogFilterByIir);

   this->Constructor_Core(instance_name);

   In_Sig = in_sig;
   Out_Sig = out_sig;
   MAKE_INPUT(In_Sig);
   MAKE_OUTPUT(Out_Sig);
   In_Sig->SetAllocMemDepth( Filt_Order );
   Using_Signal_Objects = true;
   return;
}
//======================================================
template <class T>
AnalogFilterByIir<T>::
         AnalogFilterByIir(  char *instance_name,
                             PracSimModel *outer_model )
                    :PracSimModel(  instance_name,
                                    outer_model )
{
   MODEL_NAME(AnalogFilterByIir);

   this->Constructor_Core(instance_name);
   Using_Signal_Objects = false;
   return;
}
//======================================================
// destructor

template <class T>
AnalogFilterByIir<T>::~AnalogFilterByIir()
{
   delete Denorm_Proto_Filt;
};

//======================================================
template <class T>
void AnalogFilterByIir<T>::
         Constructor_Core( char *instance_name)
{
   OPEN_PARM_BLOCK;

   Denorm_Proto_Filt = NULL;
   Lowpass_Proto_Filt = NULL;
   A_Coef_Precess_Buf = NULL;
   B_Coef_Precess_Buf = NULL;
   In_Mem = NULL;
   Out_Mem = NULL;
   Filt_Order = 0;
   Estim_Delay = 0;

   GET_BOOL_PARM(Bypass_Enabled);
   if(Bypass_Enabled){
      Filt_Band_Config = LOWPASS_FILT_BAND_CONFIG;
      Filt_Order = 0;
      Norm_Hz_Pass_Edge = 0.0;
      Norm_Hz_Pass_Edge_2 = 0.0;
      Lowpass_Proto_Filt = NULL;
      Denorm_Proto_Filt = NULL;
   }
   else{
      GET_INT_PARM(Filt_Order);
      Filt_Band_Config = 
         GetFiltBandConfigParm("Filt_Band_Config\0");

      switch (Filt_Band_Config){
      case BANDPASS_FILT_BAND_CONFIG:
      case BANDSTOP_FILT_BAND_CONFIG:
         GET_DOUBLE_PARM(Norm_Hz_Pass_Edge);
         GET_DOUBLE_PARM(Norm_Hz_Pass_Edge_2);
         if( Norm_Hz_Pass_Edge >= Norm_Hz_Pass_Edge_2){
            // error
         }
         if( Filt_Order % 2 != 0 ){
            // another error
         }
         Prototype_Order = Filt_Order/2;
         break;
      case LOWPASS_FILT_BAND_CONFIG:
      case HIGHPASS_FILT_BAND_CONFIG:
         GET_DOUBLE_PARM(Norm_Hz_Pass_Edge);
         Norm_Hz_Pass_Edge_2 = 0;
         Prototype_Order = Filt_Order;
         break;
      } // end of switch on Filter_Band_Config

      GET_BOOL_PARM(Resp_Plot_Enabled);
      if(Resp_Plot_Enabled){
         GET_BOOL_PARM(Db_Scale_Enabled);
      }
   }
   Cumul_Samp_Count = 0;
   return;
};
//======================================================
template <class T>
void AnalogFilterByIir<T>::
         Initialize( int proc_block_size, 
                     double samp_intvl)
{
   Proc_Block_Size = proc_block_size;
   Samp_Intvl = samp_intvl;
   Init_Kernel();
}
//======================================================
template <class T>
void AnalogFilterByIir<T>::Initialize()
{

   Samp_Intvl = In_Sig->GetSampIntvl();
   Proc_Block_Size = In_Sig->GetBlockSize();
   Init_Kernel();
}
//======================================================
template <class T>
void AnalogFilterByIir<T>::Init_Kernel()
{
   Input_Write_Idx = 0;
   Output_Write_Idx = 1;
   if(Bypass_Enabled) return;
   Warped_Rad_Pass_Edge =
      tan( PI * Samp_Intvl * Norm_Hz_Pass_Edge )/
      (PI * Samp_Intvl);
   Warped_Rad_Pass_Edge_2 =
      tan( PI * Samp_Intvl * Norm_Hz_Pass_Edge_2 )/
      (PI * Samp_Intvl);

   Denorm_Proto_Filt = 
         new DenormalizedPrototype(  
                              Lowpass_Proto_Filt,
                              Filt_Band_Config,
                              Warped_Rad_Pass_Edge,
                              Warped_Rad_Pass_Edge_2);

   BilinearTransf(   Denorm_Proto_Filt,
                     Samp_Intvl,
                     &A_Coefs,
                     &B_Coefs);

   Input_Buffer = new std::complex<double>[Filt_Order+1];
   Output_Buffer = new std::complex<double>[Filt_Order+1];

   for(int idx=0; idx<=Filt_Order; idx++){
      Output_Buffer[idx] = 0.0;
      Input_Buffer[idx] = 0.0;
   }

   Input_Write_Idx = 0;
   Output_Write_Idx = 1;

   IirComputedResponse *computed_response;

   if(Resp_Plot_Enabled){
      computed_response =  
         new IirComputedResponse(B_Coefs,
                                 A_Coefs,
                                 Filt_Order,
                                 Samp_Intvl,
                                 8192, //num_resp_pts
                                 Db_Scale_Enabled);

      char resp_file_name[50];
      strcpy(resp_file_name, GetModelName());
      strcat(resp_file_name, "_resp.txt\0");

      ofstream *resp_file = 
               new ofstream(resp_file_name, ios::out);
      computed_response->DumpMagResp(resp_file);
      resp_file->close();
      delete resp_file;
      delete computed_response;
   }

   Out_Mem = 
      new complex<double>[Proc_Block_Size+Filt_Order];
   int i;
   for(i=0; i<(Proc_Block_Size+Filt_Order); i++){
      *(Out_Mem+i) = 0.0;
   }

   if(Using_Signal_Objects){
      T *in_sig_ptr = GET_INPUT_PTR(In_Sig);
      in_sig_ptr -= Filt_Order;
      for(i=0; i<(Proc_Block_Size+Filt_Order); i++){
         (*in_sig_ptr) = 0.0;
         in_sig_ptr++;
      }
   }
}

//===========================================
template <class T>
int AnalogFilterByIir<T>::Execute()
{
   T *in_sig_ptr;
   T *offset_in_sig_ptr;
   complex<double> in_sig_samp;
   T *out_sig_ptr;
   complex<double> *out_mem_base;
   complex<double> *out_mem_ptr;
   complex<double> sum;
   int samp_idx;
   int n, m;
   int proc_block_size;

   in_sig_ptr = GET_INPUT_PTR(In_Sig);
   out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);

   proc_block_size = In_Sig->GetValidBlockSize();
   Out_Sig->SetValidBlockSize(proc_block_size);

   if(Bypass_Enabled){
      // copy input signal to output signal
      for(  samp_idx=0; 
            samp_idx<Proc_Block_Size; 
            samp_idx++){
         *out_sig_ptr = *in_sig_ptr;
         out_sig_ptr++;
         in_sig_ptr++;
      }
   }
   else
   {
      out_mem_base = Out_Mem;

      //  copy last N = Filt_Order samples of remembered
      //  output from previous pass down into first N 
      //  locs of double precision memory buffer

      out_mem_ptr = out_mem_base;

      for(samp_idx=0; samp_idx<Filt_Order; samp_idx++){
         *out_mem_ptr = *(out_mem_ptr+proc_block_size);
         out_mem_ptr++;
      }

      //  perform filter summations
      for(  samp_idx=0; 
            samp_idx<proc_block_size; 
            samp_idx++){
         out_mem_ptr = out_mem_base + samp_idx;
         offset_in_sig_ptr = in_sig_ptr - Filt_Order;

         sum = 0.0;
         for(n=Filt_Order; n>=1; n--){
            sum += A_Coefs[n] * (*out_mem_ptr);
            out_mem_ptr++;
         }
         for(m=Filt_Order; m>=0; m--){
            in_sig_samp = *offset_in_sig_ptr;
            sum += B_Coefs[m] * in_sig_samp;
            offset_in_sig_ptr++;
         }
         *out_mem_ptr = sum;
         if(typeid(*out_sig_ptr) == typeid(sum)){
            //*out_sig_ptr = sum;
         }
         else{
            *out_sig_ptr = sum.real();
         }

         out_sig_ptr++;
         in_sig_ptr++;
         Cumul_Samp_Count++;
      } // end of loop over samp_idx
   } // end of else clause on if(Bypass_Enabled)
   return(_MES_AOK);
}
//======================================================
//  Method to execute the model on single sample
//  in subordinate instance
template <class T>
T AnalogFilterByIir<T>::ProcessSample(T input_val)
{
   T output_val;
   std::complex<double> sum, term;
   int input_read_idx;
   int output_read_idx;
   int tap_idx;

   if(Bypass_Enabled){ 
      // copy input signal to output signal
      output_val = input_val;
   }
   else{
      Input_Buffer[Input_Write_Idx] = input_val;
      input_read_idx = Input_Write_Idx;

      Input_Write_Idx++;
      if(Input_Write_Idx > Filt_Order) 
                           Input_Write_Idx = 0;

      //  perform filter summations
      sum = 0.0;
      for( tap_idx=0; tap_idx<=Filt_Order; tap_idx++){
         term = B_Coefs[tap_idx] * 
                        Input_Buffer[input_read_idx];
         sum += term;
         input_read_idx--;
         if(input_read_idx < 0) 
                        input_read_idx = Filt_Order;
      }
      output_read_idx = Output_Write_Idx;

      for(tap_idx=1; tap_idx<=Filt_Order; tap_idx++){
         term = A_Coefs[tap_idx] * 
                        Output_Buffer[output_read_idx];
         sum+= term;
         output_read_idx--;
         if(output_read_idx < 1) 
                          output_read_idx = Filt_Order;
      }
      Output_Write_Idx++;
      if(Output_Write_Idx > Filt_Order) 
                                 Output_Write_Idx = 1;
      Output_Buffer[Output_Write_Idx] = sum;
      if(typeid(output_val) == typeid(sum)){
         //output_val = sum;
      }
      else{
         output_val = sum.real();
      }
   } // end of else clause on if(Bypass_Enabled) control structure
   return(output_val);
}
template AnalogFilterByIir<std::complex<float> >;
template AnalogFilterByIir<float>;