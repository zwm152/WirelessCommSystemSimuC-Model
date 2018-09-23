//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = anlg_filt_intg.cpp
//
//  The class AnalogFilterByInteg serves as the base class
//  for all the "traditional" analog filter types
//  e.g. Butterworth, Chebyshev, etc. that are modeled 
//  using numerical integration
//

#include <math.h>
#include "misdefs.h"
#include "parmfile.h"
#include "model_graph.h"
//#include "iir_filt.h"
#include "anlg_filt_intg.h"
#include "bilin_transf.h"
#include "iir_comp_resp.h"

extern ParmFile *ParmInput;

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================
//  constructor

AnalogFilterByInteg::AnalogFilterByInteg( char *instance_name,
                        PracSimModel *outer_model,
                        Signal<float> *in_sig,
                        Signal<float> *out_sig )
           :PracSimModel( instance_name,
                          outer_model )
{
  OPEN_PARM_BLOCK;

  In_Sig = in_sig;
  Out_Sig = out_sig;
  Denorm_Proto_Filt = NULL;
  Lowpass_Proto_Filt = NULL;
  A_Coefs = NULL;
  B_Coefs = NULL;
  //In_Mem = NULL;
  //Out_Mem = NULL;
  Filt_Order = 0;
  Estim_Delay = 0;

  GET_BOOL_PARM(Bypass_Enabled);
  if(Bypass_Enabled)
    {
    Filt_Band_Config = LOWPASS_FILT_BAND_CONFIG;
    Filt_Order = 0;
    Norm_Hz_Pass_Edge = 0.0;
    Norm_Hz_Pass_Edge_2 = 0.0;
    Lowpass_Proto_Filt = NULL;
    Denorm_Proto_Filt = NULL;
    }
  else
    {
    GET_BOOL_PARM(Using_Biquads);
    GET_INT_PARM(Filt_Order);
    Filt_Band_Config = GetFiltBandConfigParm("Filt_Band_Config\0");
    GET_DOUBLE_PARM(Integ_Alpha);

    switch (Filt_Band_Config)
      {
      case LOWPASS_FILT_BAND_CONFIG:
        GET_DOUBLE_PARM(Norm_Hz_Pass_Edge);
        Norm_Hz_Pass_Edge_2 = 0;
        Prototype_Order = Filt_Order;
        break;
      case HIGHPASS_FILT_BAND_CONFIG:
        GET_DOUBLE_PARM(Norm_Hz_Pass_Edge);
        Norm_Hz_Pass_Edge_2 = 0;
        Prototype_Order = Filt_Order;
        break;
      case BANDPASS_FILT_BAND_CONFIG:
      case BANDSTOP_FILT_BAND_CONFIG:
        {
        GET_DOUBLE_PARM(Norm_Hz_Pass_Edge);
        GET_DOUBLE_PARM(Norm_Hz_Pass_Edge_2);
        if( Norm_Hz_Pass_Edge >= Norm_Hz_Pass_Edge_2)
          {
          // error
          }
        if( Filt_Order % 2 != 0 )
          {
          // another error
          }
        Prototype_Order = Filt_Order/2;
        break;
        } // end of cases for bandpass and bandstop
      } // end of switch on Filter_Band_Config
    GET_BOOL_PARM(Resp_Plot_Enabled);
    if(Resp_Plot_Enabled)
      {
      GET_BOOL_PARM(Db_Scale_Enabled);
      }
    }
  MAKE_INPUT(In_Sig);
  MAKE_OUTPUT(Out_Sig);
  Cumul_Samp_Count = 0;

  return;
}
//=============================================================================
// destructor

AnalogFilterByInteg::~AnalogFilterByInteg(){};

//=============================================================================
void AnalogFilterByInteg::Initialize(int proc_block_size, double samp_intvl)
{
  Proc_Block_Size = proc_block_size;
  Samp_Intvl = samp_intvl;
  Init_Kernel();
}
//=============================================================================
void AnalogFilterByInteg::Initialize()
{

  Samp_Intvl = In_Sig->GetSampIntvl();
  Proc_Block_Size = In_Sig->GetBlockSize();
  Init_Kernel();
}
//=============================================================================
void AnalogFilterByInteg::Init_Kernel()
{
  Warped_Lower_Passband_Edge = Norm_Hz_Pass_Edge;
  Warped_Upper_Passband_Edge = Norm_Hz_Pass_Edge_2;
  //Warped_Lower_Passband_Edge =
  //          tan( PI * Samp_Intvl * Norm_Hz_Pass_Edge )/
  //          (PI * Samp_Intvl);
  //Warped_Upper_Passband_Edge =
  //          tan( PI * Samp_Intvl * Norm_Hz_Pass_Edge_2 )/
  //          (PI * Samp_Intvl);

  Denorm_Proto_Filt = new DenormalizedPrototype(  Lowpass_Proto_Filt,
                                                  Filt_Band_Config,
                                                  Warped_Lower_Passband_Edge,
                                                  Warped_Upper_Passband_Edge);


   if(Using_Biquads)
   {
      Integrator_0 = new NumericInteg*[Num_Biquads];
      Integrator_1 = new NumericInteg*[Num_Biquads];
      W0_Prime = new double[Num_Biquads];
      W1_Prime = new double[Num_Biquads];
      W2_Prime = new double[Num_Biquads];
      B0_Coef = new double[Num_Biquads];
      B1_Coef = new double[Num_Biquads];
      A0_Coef = new double[Num_Biquads];
      A1_Coef = new double[Num_Biquads];
      A2_Coef = new double[Num_Biquads];
   }
   else
   {  
      Integrator = new NumericInteg*[Filt_Order];
      Y_Prime = new double[Filt_Order+1];
      B_Coefs = new double[Filt_Order+1];
      A_Coefs = new double[Filt_Order];
   }

  H_Zero = Denorm_Proto_Filt->GetHZero();
  //H_Zero = Lowpass_Proto_Filt->GetHZero();
  #ifdef _DEBUG
    *DebugFile << "H_Zero = " << H_Zero << endl;
  #endif

  int k;
  *DebugFile << "in AnalofFilterByInteg, Lowpass B_coeffs:" << endl;
  Lowpass_Proto_Filt->GetDenomPoly(false);
  *DebugFile << "check point A" << endl;
  for( k=0; k<Filt_Order; k++ )
    {
    *DebugFile << "k = " << k << endl;
    Integrator[k] = new NumericInteg(Samp_Intvl, Integ_Alpha);
    B_Coefs[k] = -(Denorm_Proto_Filt->GetDenomPoly(true).GetCoefficient(k));
    //B_Coefs[k] = -(Lowpass_Proto_Filt->GetDenomPoly(false).GetCoefficient(k));
    Y_Prime[k] = 0.0;
    #ifdef _DEBUG
      *DebugFile << "in AnalogFilterByInteg, B_Coefs["
                << k << "] = " << B_Coefs[k] << endl;
    #endif
    }
  
  #ifdef _DEBUG
    int idbg;
//    for(idbg=1; idbg<=Filt_Order; idbg++)
//      {
//      *DebugFile << "A_Coefs[" << idbg << "] = " << A_Coefs[idbg] << endl;
//      }
    for(idbg=0; idbg<=Filt_Order; idbg++)
      {
      *DebugFile << "B_Coefs[" << idbg << "] = " << B_Coefs[idbg] << endl;
      }
  #endif

   Num_Zeros = Denorm_Proto_Filt->GetNumZeros();
   if(Num_Zeros == 0)
   {
      for( k=1; k<Filt_Order; k++)
      {
         A_Coefs[k] = 0.0;
      }
      A_Coefs[0] = 1.0;
   }
   else
   {
      for( k=0; k<=Num_Zeros; k++)
      {
         A_Coefs[k] = (Denorm_Proto_Filt->GetNumerPoly(true).GetCoefficient(k));
      }
   }

}

//===========================================
//  Method to execute the model
int AnalogFilterByInteg::Execute()
{
   float *in_sig_ptr;
   float *out_sig_ptr;
   double sum, output;
   int samp_idx;
   int sec, k;
   int proc_block_size;

   in_sig_ptr = GET_INPUT_PTR(In_Sig);
   out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);

  proc_block_size = In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(proc_block_size);

   if(Bypass_Enabled)
   { // copy input signal to output signal
      for(samp_idx=0; samp_idx<Proc_Block_Size; samp_idx++)
      {
         *out_sig_ptr = *in_sig_ptr;
         out_sig_ptr++;
         in_sig_ptr++;
      }
   }
   else if(Using_Biquads)
   {
      for(samp_idx=0; samp_idx<Proc_Block_Size; samp_idx++)
      {
         sum = (*in_sig_ptr) * H_Zero;
         for(sec=0; sec<Num_Biquads; sec++)
         {
            sum += W0_Prime[sec] * B0_Coef[sec];
            sum += W1_Prime[sec] * B1_Coef[sec];
            W2_Prime[sec] = sum;
            W1_Prime[sec] = Integrator_1[sec]->Integrate(W2_Prime[sec]);
            W0_Prime[sec] = Integrator_0[sec]->Integrate(W1_Prime[sec]);
         }
         sum = A0_Coef[sec] * W0_Prime[sec]
                  + A1_Coef[sec] * W1_Prime[sec]
                  + A2_Coef[sec] * W2_Prime[sec];
         *out_sig_ptr = sum;

         out_sig_ptr++;
         in_sig_ptr++;
         Cumul_Samp_Count++;
      }
   }
   else // using individual coefficients
   {
      proc_block_size = Proc_Block_Size;

      //  perform filter summations

      for(samp_idx=0; samp_idx<proc_block_size; samp_idx++)
      {
         sum = (*in_sig_ptr) * H_Zero;
         for(k=0; k<Filt_Order; k++)
         {
            sum += (Y_Prime[k] * B_Coefs[k]);
         }
         Y_Prime[Filt_Order] = sum;

         output = 0.0;
         for( k=Filt_Order-1; k>=0; k--)
         {
            Y_Prime[k] = ((Integrator[k])->Integrate(Y_Prime[k+1]));
            output += (Y_Prime[k] * A_Coefs[k]);
         }

         //*out_sig_ptr = H_Zero * output;
         *out_sig_ptr = output;

         out_sig_ptr++;
         in_sig_ptr++;
         Cumul_Samp_Count++;
      } // end of loop over samp_idx
   } // end of else clause on if(Bypass_Enabled) control structure
  return(_MES_AOK);
}