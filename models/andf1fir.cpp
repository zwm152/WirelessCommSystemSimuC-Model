//
//  File = andf1fir.cpp
//

#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "parmfile.h"
#include "andf1fir.h"
#include "fir_resp_w_noise_bw.h"
#include "model_graph.h"
#include "sigstuff.h"
#include "syst_graph.h"
#include "misdefs.h"
extern ParmFile *ParmInput;
//extern ActiveSystemGraph ActSystGraph;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

AnlgDirectFormFir::AnlgDirectFormFir( char* instance_name,
                              PracSimModel* outer_model,
                              Signal<float>* in_sig,
                              Signal<float>* out_sig )
              :PracSimModel(instance_name,
                            outer_model)
{
  In_Sig = in_sig;
  Out_Sig = out_sig;

  OPEN_PARM_BLOCK;

   GET_BOOL_PARM(Bypass_Enabled);
  GET_INT_PARM( Kind_Of_Filter_Resp );
  GET_DOUBLE_PARM( Fixed_Gain );

   if(Kind_Of_Filter_Resp == 3) {
      Coeff_Fname = new char[64];
      strcpy(Coeff_Fname, "\0");
      GET_STRING_PARM(Coeff_Fname);
   }

  MAKE_OUTPUT(Out_Sig);
  MAKE_INPUT(In_Sig);
  SAME_RATE(In_Sig, Out_Sig);

}
//=============================================
AnlgDirectFormFir::~AnlgDirectFormFir( void ){ };
//===========================================
void AnlgDirectFormFir::Initialize(void)
{
  *DebugFile << "Now in AnlgDirectFormFir::Initialize()" << endl;

  Proc_Block_Size = In_Sig->GetBlockSize();
  Samp_Intvl = In_Sig->GetSampIntvl();
  char subord_name[40];
  char response_fnam[120];
  strcpy(subord_name,GetModelName());
  strcat(subord_name,":fir_design\0");
   double *coeff;
  int i;

  //Kind_Of_Filter_Resp = 0;
  switch (Kind_Of_Filter_Resp)
    {
    case 0: // raised cosine
      Filter_Design = new FirFilterDesign(  subord_name, 
                                            Kind_Of_Filter_Resp,
                                            Samp_Intvl);
      //DebugFile << "Samp_Intvl = " << Samp_Intvl << endl;
      //Filter_Design->DumpCoefficients(&DebugFile);
      //exit(0);
      break;
    case 1: // root raised cosine
      Filter_Design = new FirFilterDesign(  subord_name, 
                                            Kind_Of_Filter_Resp,
                                            Samp_Intvl);
      //DebugFile << "Samp_Intvl = " << Samp_Intvl << endl;
      //Filter_Design->DumpCoefficients(&DebugFile);
      //exit(0);
      break;
    case 2: // pure delay
      Filter_Design = new FirFilterDesign(  subord_name, 
                                            Kind_Of_Filter_Resp,
                                            Samp_Intvl);
      //Filter_Design->DumpCoefficients(&DebugFile);
      //exit(0);
      break;
   case 3: // custom
/*      coeff[0] = coeff[24] = -0.00406909;
      coeff[1] = coeff[23] = -0.0103673;
      coeff[2] = coeff[22] = -0.00180157;
      coeff[3] = coeff[21] = 0.0152348;
      coeff[4] = coeff[20] = 0.00321361;
      coeff[5] = coeff[19] = -0.0275724;
      coeff[6] = coeff[18] = -0.00511918;
      coeff[7] = coeff[17] = 0.0494654;
      coeff[8] = coeff[16] = 0.00700851;
      coeff[9] = coeff[15] = -0.0969915;
      coeff[10] = coeff[14] = -0.00831958;
      coeff[11] = coeff[13] = 0.315158;
      coeff[12] = 0.50881;
      */
/*      coeff[0] = coeff[18] = -0.033637;
      coeff[1] = coeff[17] = -0.023387;
      coeff[2] = coeff[16] = 0.026728;
      coeff[3] = coeff[15] = 0.050455;
      coeff[4] = coeff[14] = 0.0;
      coeff[5] = coeff[13] = 0.075683;
      coeff[6] = coeff[12] = -0.062366;
      coeff[7] = coeff[11] = 0.093549;
      coeff[8] = coeff[10] = 0.302731;
      coeff[9] = 0.40;
      */

      Coeff_File = new ifstream(Coeff_Fname, ios::in);
      *Coeff_File >> Num_Taps;

      coeff = new double[Num_Taps];

      for( i=0; i<Num_Taps; i++) {
         *Coeff_File >> coeff[i];
      }


      Filter_Design = new FirFilterDesign(  Num_Taps, 
                                            coeff);      
      break;
    default:
      // error 
      cout << "illegal value for 'Kind_Of_Filter_Resp' " << endl;
      exit(1);
    } // end of switch on Kind_Of_Filter_Resp
  Num_Taps = Filter_Design->GetNumTaps();
  Coeff = new float[Num_Taps];
  Filter_Design->CopyCoefficients(Coeff);
  Input_Mem = new float[Num_Taps];
  for(int n=0; n< Num_Taps; n++) Input_Mem[n] = 0.0;
  //
  //---------------------------------------------
  // compute magnitude response of filter and write to a file
  response_fnam[0] = 0;
  strcpy(response_fnam,"resp_\0");
  strcat(response_fnam,GetInstanceName());
  strcat(response_fnam,".txt\0");
  FirFilterResponse* response = 
            new FirFilterResponse(  Filter_Design,
                                    500,
                                    1, //dB scale enabled
                                    1, //normalization enabled
                                    response_fnam);
   response->ComputeMagResp();
   response->DumpMagResp();
}
//=============================================
int AnlgDirectFormFir::Execute(void)
{
  *DebugFile << "In AnlgDirectFormFir::Execute\0" << endl;

  float* input_mem = Input_Mem;
  float* coeff = Coeff;
  int num_taps = Num_Taps;
  float *out_sig, *in_sig;
  int is, tap_num;
  float sum;

  Proc_Block_Size = In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(Proc_Block_Size);

  out_sig = GET_OUTPUT_PTR(Out_Sig);
  in_sig = GET_INPUT_PTR(In_Sig);

   if(Bypass_Enabled) {
      for( is=0; is<Proc_Block_Size; is++) {
         *out_sig++ = *in_sig++;
      }
   }
   else {
      for ( is=0; is<Proc_Block_Size; is++) {
         for( tap_num = num_taps-1; tap_num > 0; tap_num--) {
            input_mem[tap_num] = input_mem[tap_num-1];
         }
         input_mem[0] = *in_sig++;
         sum = 0.0;
         for( tap_num = 0; tap_num < num_taps; tap_num++) {
            sum += coeff[tap_num] * input_mem[tap_num];
         }

         *out_sig++ = float(Fixed_Gain * sum);
      }
   }
   return(_MES_AOK);
}

