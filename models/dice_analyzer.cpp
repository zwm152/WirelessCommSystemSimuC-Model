//
//  File = dice_analyzer.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "dice_analyzer.h"
#include "typedefs.h"
#include "model_graph.h"
#include "syst_graph.h"
extern ParmFile *ParmInput;
extern ofstream *DebugFile;
extern SystemGraph CommSystemGraph;
extern int PassNumber;
extern int MaxPassNumber;

//======================================================

DiceAnalyzer::DiceAnalyzer( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<byte_t>* symb_seq )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(DiceAnalyzer);
  In_Sig = symb_seq;

  OPEN_PARM_BLOCK;
  GET_INT_PARM(Report_Intvl_In_Blocks);
  GET_INT_PARM(Faces_Per_Die);

  MAKE_INPUT(In_Sig);

  Symb_Count = new int[Faces_Per_Die];
  Double_Count = new int[Faces_Per_Die*Faces_Per_Die];

}
//======================================================
DiceAnalyzer::~DiceAnalyzer( void ){ };

//======================================================
void DiceAnalyzer::Initialize(void)
{
   int i;
   Block_Size = In_Sig->GetBlockSize();

   for(i=0; i<Faces_Per_Die; i++)
      Symb_Count[i] = 0;
   for(i=0; i<Faces_Per_Die*Faces_Per_Die; i++)
      Double_Count[i] = 0;
};

//======================================================
int DiceAnalyzer::Execute()
{
   byte_t *in_byte_ptr, in_byte;
   byte_t old_byte;
   int is, i,j;

   in_byte_ptr = GET_INPUT_PTR( In_Sig );

   for( is=0; is < Block_Size; is++)
   {
      in_byte = *in_byte_ptr++;
      Symb_Count[in_byte-1]++;
      if(is%2)
      {
         Double_Count[old_byte*Faces_Per_Die + in_byte-1]++;
      }
      else
      {
         old_byte = in_byte-1;
      }
     

   }
   if( (PassNumber % Report_Intvl_In_Blocks) == 0)
   {
      float total_count = float(Symb_Count[0]);
      for( i=1; i<Faces_Per_Die; i++)
         total_count += Symb_Count[i];

      BasicResults << Instance_Name << ": "
              << PassNumber << "  symbol %'s: "
              << (Symb_Count[0]/total_count);

      for( i=1; i<Faces_Per_Die; i++)
      {
         BasicResults << ", " << (Symb_Count[i]/total_count);
      }
      BasicResults << "\n" << endl;

      total_count = float(Double_Count[0]);
      for( i=1; i<Faces_Per_Die*Faces_Per_Die; i++)
         total_count += Double_Count[i];


      for( i=0; i<Faces_Per_Die; i++)
      {
         BasicResults << (Double_Count[i*Faces_Per_Die]/total_count);
         for( j=1; j<Faces_Per_Die; j++)
         {
            BasicResults << ", " << (Double_Count[i*Faces_Per_Die+j]/total_count);
         }
         BasicResults << "\n" << endl;
      }
   }

   return(_MES_AOK);
}

