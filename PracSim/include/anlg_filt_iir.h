//
//  File = anlg_filt_iir.h
//

#ifndef _ANLG_FILT_IIR_H_
#define _ANLG_FILT_IIR_H_

#include <complex>
#include "signal_T.h"
#include "filter_proto.h"
#include "denorm_proto.h"
#include "psmodel.h"
#include "filter_types.h"
//======================================================
template <class T>
class AnalogFilterByIir : public PracSimModel
{
public:

   // Normal Constructor
   AnalogFilterByIir(   char* instance_name,
                        PracSimModel *outer_model,
                        Signal<T> *in_sig,
                        Signal<T> *out_sig );

   // Subordinate Constructor
   AnalogFilterByIir(   char* instance_name,
                        PracSimModel *outer_model);

   ~AnalogFilterByIir(void);

   // Normal Initialize
   virtual void Initialize(void);

   // Subordinate Initialize
   void Initialize(  int proc_block_size, 
                     double samp_intvl);

   virtual int Execute();

   // Used instead of 'Execute' for subordinate form
   T ProcessSample(T input_val);

protected:

   // Implements functionality common to both ctors
   void Constructor_Core( char *instance_name);

   // Implements functionality common to both
   // forms of 'Initialize'
   void Init_Kernel(void);

   Signal<T> *In_Sig;
   Signal<T> *Out_Sig;

   float Estim_Delay;

   std::complex<double> *Out_Mem;
   std::complex<double> *In_Mem;

   std::complex<double> *Input_Buffer;
   std::complex<double> *Output_Buffer;

   int Input_Write_Idx;
   int Output_Write_Idx;

   bool Bypass_Enabled;

   double Samp_Rate;
   double Samp_Intvl;

   DenormalizedPrototype *Denorm_Proto_Filt;
   AnalogFilterPrototype *Lowpass_Proto_Filt;

   int Filt_Shape;

   FILT_BAND_CONFIG_T Filt_Band_Config;

   int Filt_Order;

   bool Resp_Plot_Enabled;
   bool Db_Scale_Enabled;

   int Prototype_Order;
   int Upper_Summation_Limit;

   float Upper_Cutoff_NFU;
   float Lower_Cutoff_NFU;

   float Passband_Ripple;
   float Stopband_Ripple;

   float Stopband_Atten;

   float Norm_Hz_Pass_Edge;
   float Norm_Hz_Pass_Edge_2;

   float Warped_Rad_Pass_Edge;
   float Warped_Rad_Pass_Edge_2;

   float Norm_Hz_Stop_Edge;
   float Lower_Stopband_Edge;

   float Upper_Trans_Width;
   float Lower_Trans_Width;

   double *A_Coefs;
   double *B_Coefs;
   double *A_Coef_Precess_Buf;
   double *B_Coef_Precess_Buf;

   int Num_Poles;
   int Num_Zeros;

   int Cumul_Samp_Count;
   int Proc_Block_Size;
   bool Using_Signal_Objects;

};

#endif
