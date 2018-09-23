//
//  File = k_pwrmtr.cpp
//

#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "parmfile.h"
#include "k_pwrmtr.h"
#include "misdefs.h"
extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
extern int PassNumber;
ofstream PowerDebug("power.txt", ios::out);

//======================================================
// constructor - parms read from ParmFile

template <class T>
k_PowerMeter<T>::k_PowerMeter( char* instance_name )
//              :PracSimModel(instance_name,
//                            outer_model)
{
//  In_Sig = in_sig;
//  Est_Avg_Sig_Pwr = est_avg_sig_pwr;

  OPEN_PARM_BLOCK;

  GET_FLOAT_PARM(Init_Pwr_Output);
  GET_FLOAT_PARM(Integ_Const);

}
//============================================
//  constructor for subordinate instance with
//              parms passed in call

template <class T>
k_PowerMeter<T>::k_PowerMeter( char *instance_name,
                            float init_pwr_output,
                            float integ_const )
//        :PracSimModel( instance_name, outer_model)
{
//  In_Sig = in_sig;
//  Est_Avg_Sig_Pwr = est_avg_sig_pwr;

  Init_Pwr_Output = init_pwr_output;
  Integ_Const = integ_const;

}
//=============================================
template <class T>
k_PowerMeter<T>::~k_PowerMeter( void ){ };

//===========================================
template <class T>
void k_PowerMeter<T>::Initialize(int max_proc_block_size,
                              double samp_intvl)
{
  *DebugFile << "Now in k_PowerMeter::Initialize()" << endl;

  Max_Proc_Block_Size = max_proc_block_size;
  Meas_Pwr_Buf = new float[Max_Proc_Block_Size];

  //double samp_intvl = 1.0/samp_rate;
  double dx = 1.0/tan(PI*samp_intvl/Integ_Const);
  Const_1 = 1.0/(dx+1.0);
  Const_2 = (dx-1.0)/(dx+1.0);
  Delay_Reg = Init_Pwr_Output/2.0;
  //Delay_Reg = Init_Pwr_Output;

}
//=============================================
template <class T>
int k_PowerMeter<T>::Execute(  T *in_sig_ptr,
                            float *est_avg_sig_pwr,
                            int block_size)
{
  int is;
  float *meas_pwr_ptr;
  float meas_pwr;
  T in_sig;
  std::complex<float> cmpx_in_sig;
  //float pwr_sig;
  //float desired_avg_pwr, gain;
  
  #ifdef _DEBUG
    *DebugFile << "In k_PowerMeter::Execute\0" << endl;
  #endif

  meas_pwr_ptr = Meas_Pwr_Buf;

  //----------------------------------------------

  for(is=0; is<block_size; is++)
    {
    in_sig = *in_sig_ptr++;
    cmpx_in_sig = in_sig;
    *meas_pwr_ptr++ = std::norm(cmpx_in_sig);
    //*meas_pwr_ptr++ = std::norm(cmpx_in_sig)/2.0;
    }
  //---------------------------------------------
  // smooth the measured signal power with an integrating filter

  double const_1 = Const_1;
  double const_2 = Const_2;
  double delay_reg = Delay_Reg;
  float result;
  double dy;
  meas_pwr_ptr = Meas_Pwr_Buf;

  for(is=0; is<block_size; is++)
    {
    meas_pwr = *meas_pwr_ptr++;
    dy = meas_pwr * const_1 + delay_reg * const_2;
    result = float(dy + delay_reg);
    delay_reg = dy;
    *est_avg_sig_pwr++ = result;
    //PowerDebug << (is + (PassNumber-1)*block_size) << ", "
    //          << meas_pwr << ", " << result << endl;
    }
  Delay_Reg = delay_reg;

  return(0);
}
template k_PowerMeter<std::complex<float> >;
template k_PowerMeter<float>;

