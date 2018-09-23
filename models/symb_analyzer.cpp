//
//  File = symb_analyzer.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "symb_analyzer.h"
#include "typedefs.h"
#include "model_graph.h"
#include "syst_graph.h"
extern ParmFile *ParmInput;
extern ofstream *DebugFile;
extern SystemGraph CommSystemGraph;
extern int PassNumber;
extern int MaxPassNumber;

//======================================================

SymbSeqAnalyzer::SymbSeqAnalyzer( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<byte_t>* symb_seq )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(SymbSeqAnalyzer);
  In_Sig = symb_seq;

  OPEN_PARM_BLOCK;
  GET_INT_PARM(Report_Intvl_In_Blocks);
  GET_INT_PARM(Bits_Per_Symb);

  MAKE_INPUT(In_Sig);

  Num_Diff_Symbs = 1<<Bits_Per_Symb;
  Symb_Count = new int[Num_Diff_Symbs];
  Double_Count = new int[Num_Diff_Symbs*Num_Diff_Symbs];

}
//======================================================
SymbSeqAnalyzer::~SymbSeqAnalyzer( void ){ };

//======================================================
void SymbSeqAnalyzer::Initialize(void)
{
   int i;
   Block_Size = In_Sig->GetBlockSize();

   for(i=0; i<Num_Diff_Symbs; i++)
      Symb_Count[i] = 0;
   for(i=0; i<Num_Diff_Symbs*Num_Diff_Symbs; i++)
      Double_Count[i] = 0;
};

//======================================================
int SymbSeqAnalyzer::Execute()
{
   byte_t *in_byte_ptr, in_byte;
   byte_t old_byte;
   int is, i,j;

   in_byte_ptr = GET_INPUT_PTR( In_Sig );

   for( is=0; is < Block_Size; is++)
   {
      in_byte = *in_byte_ptr++;
      Symb_Count[in_byte]++;
      if(is%2)
      {
         Double_Count[old_byte*Num_Diff_Symbs + in_byte]++;
      }
      else
      {
         old_byte = in_byte;
      }
     

   }
   if( (PassNumber % Report_Intvl_In_Blocks) == 0)
   {
      float total_count = float(Symb_Count[0]);
      for( i=1; i<Num_Diff_Symbs; i++)
         total_count += Symb_Count[i];

      BasicResults << Instance_Name << ": "
              << PassNumber << "  symbol %'s: "
              << (Symb_Count[0]/total_count);

      for( i=1; i<Num_Diff_Symbs; i++)
      {
         BasicResults << ", " << (Symb_Count[i]/total_count);
      }
      BasicResults << "\n" << endl;

      total_count = float(Double_Count[0]);
      for( i=1; i<Num_Diff_Symbs*Num_Diff_Symbs; i++)
         total_count += Double_Count[i];

      for( i=0; i<Num_Diff_Symbs; i++)
      {
         for( j=0; j<Num_Diff_Symbs; j++)
         {
            BasicResults << ", " << (Double_Count[i*Num_Diff_Symbs+j]/total_count);
         }
         BasicResults << "\n" << endl;
      }
   }

   return(_MES_AOK);
}

