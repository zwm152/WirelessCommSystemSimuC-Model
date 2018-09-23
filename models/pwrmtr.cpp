//
//  File = pwrmtr.cpp
//

#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "parmfile.h"
#include "pwrmtr.h"
#include "model_graph.h"
#include "sigstuff.h"
#include "syst_graph.h"
#include "misdefs.h"
#include "gensig.h"
extern ParmFile *ParmInput;
///extern ActiveSystemGraph ActSystGraph;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================
// normal constructor

PowerMeter::PowerMeter( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<float>* in_sig,
                        Signal<float>* est_avg_sig_pwr)
              :PracSimModel(instance_name,
                            outer_model)
{
  In_Sig = in_sig;
  Est_Avg_Sig_Pwr = est_avg_sig_pwr;

  OPEN_PARM_BLOCK;

  float init_pwr_output;
  GET_FLOAT_PARM(init_pwr_output);

  float integ_const;
  GET_FLOAT_PARM(integ_const);
  
  char sub_name[50];
  strcpy(sub_name,GetModelName());
  strcat(sub_name, ":Kernel\0");
  Kernel = new k_PowerMeter<float>( sub_name,
                                 init_pwr_output,
                                 integ_const);

  Setup();
}
//============================================
//  constructor for subordinate instance
PowerMeter::PowerMeter( char *instance_name,
                        PracSimModel *outer_model,
                        Signal<float> *in_sig,
                        Signal<float> *est_avg_sig_pwr,
                        float init_pwr_output,
                        float integ_const )
        :PracSimModel( instance_name, outer_model)
{
  In_Sig = in_sig;
  Est_Avg_Sig_Pwr = est_avg_sig_pwr;

  char sub_name[50];
  strcpy(sub_name,GetModelName());
  strcat(sub_name, ":Kernel\0");
  Kernel = new k_PowerMeter<float>( sub_name,
                             init_pwr_output,
                             integ_const);

  Setup();
}
//=============================================
PowerMeter::~PowerMeter( void ){ };
//=============================================
void PowerMeter::Setup( void )
{
  MAKE_OUTPUT(Est_Avg_Sig_Pwr);

  MAKE_INPUT(In_Sig);

  SAME_RATE(In_Sig, Est_Avg_Sig_Pwr);

};
//===========================================
void PowerMeter::Initialize(void)
{
  *DebugFile << "Now in CmpxPowerMeter::Initialize()" << endl;

  Proc_Block_Size = In_Sig->GetBlockSize();

  Kernel->Initialize( Proc_Block_Size,
                      In_Sig->GetSampIntvl());

}
//=============================================
int PowerMeter::Execute(void)
{
  int pm_status;
  float *out_sig_ptr;
  float *in_sig_ptr;
  int block_size;

  *DebugFile << "In PowerMeter::Execute\0" << endl;
  block_size = In_Sig->GetValidBlockSize();
  Est_Avg_Sig_Pwr->SetValidBlockSize( block_size );

  in_sig_ptr = GET_INPUT_PTR(In_Sig);
  out_sig_ptr = GET_OUTPUT_PTR(Est_Avg_Sig_Pwr);

  //----------------------------------------------

  pm_status = Kernel->Execute(  in_sig_ptr,
                                out_sig_ptr,
                                block_size);

  return(_MES_AOK);
}

