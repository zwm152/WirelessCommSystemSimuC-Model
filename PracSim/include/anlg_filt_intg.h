//
//  File = anlg_filt_intg.h
//

#ifndef _ANLG_FILT_INTG_H_
#define _ANLG_FILT_INTG_H_

#include <complex>
#include "signal_T.h"
#include "filter_proto.h"
#include "denorm_proto.h"
#include "numinteg.h"
#include "psmodel.h"
#include "filter_types.h"

class AnalogFilterByInteg : public PracSimModel
{
public:

  // Constructor
  AnalogFilterByInteg( char* instance_name,
              PracSimModel *outer_model,
              Signal<float> *in_sig,
              Signal<float> *out_sig );

  //AnalogFilter( char *instance_name,
  //            int kind_of_interface,
  //            int filter_band_config,
  //            int filter_order,
  //            float upper_passband_edge,
  //            float lower_passband_edge,
  //            bool bypass_enabled);

  ~AnalogFilterByInteg(void);

  virtual void Initialize(void);
  void Initialize(int proc_block_size, double samp_intvl);

  virtual int Execute();

protected:
  void Init_Kernel(void);

  Signal<float> *In_Sig;

  Signal<float> *Out_Sig;

  float Estim_Delay;

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
  float Norm_Hz_Stop_Edge;
  float Norm_Hz_Pass_Edge_2;

  float Warped_Upper_Passband_Edge;
  float Warped_Lower_Passband_Edge;
  
  float Upper_Stopband_Edge;
  float Lower_Stopband_Edge;

  float Upper_Trans_Width;
  float Lower_Trans_Width;

   NumericInteg** Integrator_0;
   NumericInteg** Integrator_1;
   double *W0_Prime;
   double *W1_Prime;
   double *W2_Prime;
   double *B0_Coef;
   double *B1_Coef;
   double *A0_Coef;
   double *A1_Coef;
   double *A2_Coef;
   bool Using_Biquads;

  double *A_Coefs;
  double *B_Coefs;
  NumericInteg** Integrator;
  double *Y_Prime;
  double H_Zero;
  double Integ_Alpha;

  int Num_Poles;
  int Num_Zeros;
  int Num_Biquads;

  int Cumul_Samp_Count;
  int Proc_Block_Size;
  
};

#endif
