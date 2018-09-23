//
//  File = bscerrorgen.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "bscerrorgen.h"
#include "uni_rand.h"
#include "model_graph.h"
#include "syst_graph.h"
#include "misdefs.h"
extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

#define FLOAT_MODULUS 2147483647.0

//======================================================

BscErrorGener::BscErrorGener( char* instance_name,
                              PracSimModel* outer_model,
                              Signal<bit_t>* in_signal,
                              Signal<bit_t>* out_signal)
              :PracSimModel(instance_name,
                            outer_model)
{
   MODEL_NAME(BscErrorGener);

   In_Sig = in_signal;
   Out_Sig = out_signal;

   OPEN_PARM_BLOCK;
   GET_DOUBLE_PARM(Bit_Error_Probability);
   Decis_Thresh = (unsigned long)
               (Bit_Error_Probability * FLOAT_MODULUS);
   BasicResults << "Decis_Thresh = " << Decis_Thresh 
                << endl;
#ifdef _DEBUG
   *DebugFile << "Decis_Thresh = " << Decis_Thresh 
              << endl;
#endif
   GET_LONG_PARM(Initial_Seed);
   Working_Seed = Initial_Seed;

   MAKE_OUTPUT( Out_Sig );
   MAKE_INPUT( In_Sig );

}

//======================================================
BscErrorGener::~BscErrorGener( void ){ };

//======================================================
void BscErrorGener::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
};

//======================================================
int BscErrorGener::Execute()
{
   bit_t *in_sig_ptr;
   bit_t output_samp, *out_sig_ptr;
   int is, block_size;
   long working_seed;
   long decis_thresh;

   // Get pointers for input and output signal buffers

   out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);
   in_sig_ptr = GET_INPUT_PTR(In_Sig);

   //  copy frequently used items into local storage

   block_size = In_Sig->GetValidBlockSize();
   Out_Sig->SetValidBlockSize(block_size);
   working_seed = Working_Seed;
   decis_thresh = Decis_Thresh;

   for(is=0; is<block_size; is++){
      output_samp = *in_sig_ptr++;

      RandomLong(&working_seed);
      if(working_seed <= decis_thresh) output_samp ^=1;
      *out_sig_ptr++ = output_samp;
   }
   Working_Seed = working_seed;
   return(_MES_AOK);
}

