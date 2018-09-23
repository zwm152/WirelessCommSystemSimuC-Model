//
//  File = k_serctr.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "k_serctr.h"
#include "typedefs.h"
#include "model_graph.h"
#include "syst_graph.h"
extern ParmFile *ParmInput;
extern ofstream *DebugFile;
extern SystemGraph CommSystGraph;
extern int PassNumber;
extern int MaxPassNumber;

//======================================================
// constructor - parms read from ParmFile

k_SerCounter::k_SerCounter( char* instance_name )
{
  Instance_Name = new char[strlen(instance_name)];
  strcpy(Instance_Name, instance_name);

  OPEN_PARM_BLOCK;
  GET_INT_PARM(Num_Holdoff_Passes);
  GET_INT_PARM(Report_Intvl_In_Blocks);

  Symb_Count = 0;
  Error_Count = 0;
}
//==============================================
k_SerCounter::k_SerCounter( char* instance_name,
                            int num_holdoff_passes,
                            int report_intvl_in_blocks)
{
  Instance_Name = new char[strlen(instance_name)];
  strcpy(Instance_Name, instance_name);

  Num_Holdoff_Passes = num_holdoff_passes;
  Report_Intvl_In_Blocks = report_intvl_in_blocks;
  Symb_Count = 0;
  Error_Count = 0;
}
//==============================================
k_SerCounter::~k_SerCounter( void ){ };

//==============================================
void k_SerCounter::Initialize(void)
{
};

//==============================================
int k_SerCounter::Execute(  byte_t *in_sig_ptr,
                            byte_t *ref_sig_ptr,
                            byte_t *error_seq_ptr,
                            int block_size )
{
  byte_t symb_val, ref_val;
  int symb_count, error_count;
  byte_t *symbs_in;
  byte_t *symbs_ref;
  byte_t *error_tags;
  int is;

  if(PassNumber < Num_Holdoff_Passes ) return(_MES_AOK);

  symbs_in = in_sig_ptr;
  symbs_ref = ref_sig_ptr;
  error_tags = error_seq_ptr;

  symb_count = Symb_Count;
  error_count = Error_Count;
  bool error_found = false;

  for (is=0; is < block_size; is++)
    {
    symb_val = *symbs_in++;
    ref_val = *symbs_ref++;
    symb_count++;
    if(symb_val != ref_val) 
      {
      error_found = true;
      *error_tags++ = 16;
      error_count++;
      }
    else
      {
      *error_tags++ = 0;
      }
    }
//  if(PassNumber == MaxPassNumber)
  if( ((PassNumber - Num_Holdoff_Passes) % Report_Intvl_In_Blocks) == 0)
    {
    BasicResults << Instance_Name << ": "
              << PassNumber << "  Ser = " << (float(error_count)/float(symb_count)) << " -- "
              << error_count << " errors in " << symb_count << " symbols" << endl;
    }
  Symb_Count = symb_count;
  Error_Count = error_count;
  return(_MES_AOK);
}

