//
//  File = disc_auto_corr.cpp
//

#include <stdlib.h>
#include "parmfile.h"
#include "disc_auto_cov.h"
#include "misdefs.h"
#include "model_graph.h"
#include "sigplot.h"
extern ParmFile* ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
ofstream CovarFile("covar_res.txt", ios::out);

//======================================================

DiscreteAutoCovar::DiscreteAutoCovar( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<byte_t>* in_sig)
            :PracSimModel(instance_name,
                          outer_model)
{
  In_Sig = in_sig;

  OPEN_PARM_BLOCK;

  GET_INT_PARM(Num_Corr_Passes);
  GET_DOUBLE_PARM(Mean);
  Mean_Sqrd = Mean * Mean;

  MAKE_INPUT(In_Sig);

}
DiscreteAutoCovar::~DiscreteAutoCovar( void ){ };

void DiscreteAutoCovar::Initialize(void)
{
   Proc_Block_Size = In_Sig->GetBlockSize();
   Samp_Intvl = In_Sig->GetSampIntvl();
   Corr_Buf = new double[Proc_Block_Size/2];
   for(int i=0; i<Proc_Block_Size/2; i++) Corr_Buf[i]=0.0;

//   Max_Corr = 0.0;
//   Max_Corr_Time = 0.0;

   //INITIALIZATION_REPORT(BasicResults);
   return;
}

int DiscreteAutoCovar::Execute()
{
   byte_t *in_sig_beg;
   byte_t *in_sig_ptr;
   byte_t *slide_sig_beg;
   byte_t *slide_sig_ptr;
   int i, j;
   int sum;
   double covar_val;

   if(PassNumber > Num_Corr_Passes) return(_MES_AOK);

   //-------------------------------------------------------
   //  Copy frequently accessed member vars into local vars
   int proc_block_size = Proc_Block_Size;
   int slide_block_size = proc_block_size/2;
   double samp_intvl = Samp_Intvl;

   //----------------------------------------
   // Get pointers for input and output
   in_sig_beg = GET_INPUT_PTR(In_Sig);
   slide_sig_beg = in_sig_beg + proc_block_size/4;

   //----------------------------------------

   for( j=0; j<slide_block_size; j++)
   {
      sum = 0;
      in_sig_ptr = in_sig_beg;
      slide_sig_ptr = slide_sig_beg;
      for( i=0; i<slide_block_size; i++)
      {
         sum += (*in_sig_ptr )*(*slide_sig_ptr );
         slide_sig_ptr++;
         in_sig_ptr++;
      }
      Corr_Buf[j] += ((float(sum)/slide_block_size)- Mean_Sqrd);
      in_sig_beg++;
   }
   if(PassNumber == Num_Corr_Passes)
   {
      for( j=0; j<slide_block_size; j++)
      {
         covar_val = fabs((Corr_Buf[j]/Num_Corr_Passes));
         if(covar_val <1e-20) covar_val = 1e-20;
         CovarFile << j-slide_block_size/2 << ", " << 10.0*log10(covar_val) << endl;
         //CovarFile << j-slide_block_size/2 << ", " << Corr_Buf[j] << endl;
      }
      CovarFile.close();
      exit(22);
   }
   //---------------------
   return(_MES_AOK);
}

