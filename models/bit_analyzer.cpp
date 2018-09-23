//
//  File = bit_analyzer.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "bit_analyzer.h"
#include "typedefs.h"
#include "model_graph.h"
#include "syst_graph.h"
extern ParmFile *ParmInput;
extern ofstream *DebugFile;
extern SystemGraph CommSystemGraph;
extern int PassNumber;
extern int MaxPassNumber;

//======================================================

BitSeqAnalyzer::BitSeqAnalyzer( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<bit_t>* bit_seq )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(BitSeqAnalyzer);
  In_Sig = bit_seq;

  OPEN_PARM_BLOCK;
  GET_INT_PARM(Report_Intvl_In_Blocks);

  MAKE_INPUT(In_Sig);

  Symb_Count = 0;

}
//======================================================
BitSeqAnalyzer::~BitSeqAnalyzer( void ){ };

//======================================================
void BitSeqAnalyzer::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Zeros_Count = 0;
  Ones_Count = 0;
};

//======================================================
int BitSeqAnalyzer::Execute()
{
   bit_t *in_bit_ptr, in_bit;
   int is;
   int ones_count, zeros_count;

   in_bit_ptr = GET_INPUT_PTR( In_Sig );
   zeros_count = Zeros_Count;
   ones_count = Ones_Count;

   for( is=0; is < Block_Size; is++)
   {
      in_bit = *in_bit_ptr++;
      if( in_bit == 0 )
         zeros_count++;
      else
         ones_count++;

   }
  if( (PassNumber % Report_Intvl_In_Blocks) == 0)
    {
    float total_count = float(ones_count + zeros_count);
    BasicResults << Instance_Name << ": "
              << PassNumber << "  zeros_percent = " 
              << (zeros_count/total_count) << " -- "
              << "  ones_percent = " 
              << (ones_count/total_count) << endl;
    }
   Ones_Count = ones_count;
   Zeros_Count = zeros_count;

   return(_MES_AOK);
}

