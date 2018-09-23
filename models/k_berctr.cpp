//
//  File = k_berctr.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "k_berctr.h"
#include "typedefs.h"
#include "model_graph.h"
#include "syst_graph.h"
extern ParmFile *ParmInput;
extern ofstream *DebugFile;
extern SystemGraph CommSystemGraph;
extern int PassNumber;
extern int MaxPassNumber;

//======================================================
// constructor - parms read from ParmFile

k_BerCounter::k_BerCounter( char* instance_name )
{
  Instance_Name = new char[strlen(instance_name)];
  strcpy(Instance_Name, instance_name);

  OPEN_PARM_BLOCK;
  GET_INT_PARM(Num_Holdoff_Passes);
  GET_INT_PARM(Report_Intvl_In_Blocks);

  Bit_Count = 0;
  Bit_0_Count = 0;
  Bit_1_Count = 0;
  Error_Count = 0;
  Error_0_Count = 0;
  Error_1_Count = 0;
}
//==============================================
k_BerCounter::k_BerCounter( char* instance_name,
                            int num_holdoff_passes,
                            int report_intvl_in_blocks)
{
  Instance_Name = new char[strlen(instance_name)];
  strcpy(Instance_Name, instance_name);

  Num_Holdoff_Passes = num_holdoff_passes;
  Report_Intvl_In_Blocks = report_intvl_in_blocks;
  Bit_Count = 0;
  Bit_0_Count = 0;
  Bit_1_Count = 0;
  Error_Count = 0;
  Error_0_Count = 0;
  Error_1_Count = 0;
}
//==============================================
k_BerCounter::~k_BerCounter( void ){ };

//==============================================
void k_BerCounter::Initialize(void)
{
};

//==============================================
int k_BerCounter::Execute( bit_t *in_sig_ptr,
                            bit_t *ref_sig_ptr,
                            int block_size )
{
  bit_t bit_val, ref_val;
  int bit_count, error_count;
  int bit_0_count, bit_1_count;
  int error_0_count, error_1_count;
  bit_t *symbs_in;
  bit_t *symbs_ref;
  int is;

  if(PassNumber <= Num_Holdoff_Passes ) return(_MES_AOK);

  symbs_in = in_sig_ptr;
  symbs_ref = ref_sig_ptr;

  bit_count = Bit_Count;
  bit_0_count = Bit_0_Count;
  bit_1_count = Bit_1_Count;

  error_count = Error_Count;
  error_0_count = Error_0_Count;
  error_1_count = Error_1_Count;

  bool error_found = false;

  for (is=0; is < block_size; is++)
    {
    bit_val = *symbs_in++;
    ref_val = *symbs_ref++;
    bit_count++;

    if(ref_val == 0)
      bit_0_count++;
    else
      bit_1_count++;

    if(bit_val != ref_val) 
      {
      error_found = true;
      error_count++;

      if(ref_val == 0)
        error_0_count++;
      else
        error_1_count++;
      }
    }
//  if(PassNumber == MaxPassNumber)
  if( ((PassNumber - Num_Holdoff_Passes) % Report_Intvl_In_Blocks) == 0)
    {
    BasicResults << Instance_Name << ": "
              << PassNumber << "  BER = " << (float(error_count)/float(bit_count)) << " -- "
              << error_count << " errors in " << bit_count << " bits" << endl;
    BasicResults << "space errors = " << error_0_count
                 << "   mark errors = " << error_1_count << endl;
    }
  Bit_Count = bit_count;
  Bit_0_Count = bit_0_count;
  Bit_1_Count = bit_1_count;

  Error_Count = error_count;
  Error_0_Count = error_0_count;
  Error_1_Count = error_1_count;
  return(_MES_AOK);
}

