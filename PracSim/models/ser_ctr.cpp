//
//  File = ser_ctr.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "ser_ctr.h"
#include "typedefs.h"
#include "model_graph.h"
#include "syst_graph.h"
extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
extern SystemGraph CommSystGraph;
extern int PassNumber;
extern int MaxPassNumber;

//======================================================

SerCounter::SerCounter( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<byte_t>* ref_signal,
                        Signal<byte_t>* in_signal,
                        Signal<byte_t>* error_seq )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(SerCounter);
  In_Sig = in_signal;
  Ref_Sig = ref_signal;
  Error_Seq = error_seq;

  OPEN_PARM_BLOCK;
  GET_INT_PARM(Num_Holdoff_Passes);
  GET_INT_PARM(Report_Intvl_In_Blocks);

  MAKE_OUTPUT(Error_Seq);
  MAKE_INPUT(In_Sig);
  MAKE_INPUT(Ref_Sig);

  Symb_Count = 0;
  Error_Count = 0;

  char sub_name[50];
  strcpy(sub_name, GetModelName());
  strcat(sub_name, ":Kernel\0");
  Kernel = new k_SerCounter(  sub_name, 
                              Num_Holdoff_Passes,
                              Report_Intvl_In_Blocks );
}
//======================================================
SerCounter::~SerCounter( void ){ };

//======================================================
void SerCounter::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
};

//======================================================
int SerCounter::Execute()
{
  byte_t *symbs_in;
  byte_t *symbs_ref;
  byte_t *error_tags;
  int block_size;

  //if(PassNumber <= 3 ) return(_MES_AOK);

  symbs_in = GET_INPUT_PTR( In_Sig );
  symbs_ref = GET_INPUT_PTR( Ref_Sig );
  error_tags = GET_OUTPUT_PTR( Error_Seq );
  block_size = In_Sig->GetValidBlockSize();
  Error_Seq->SetValidBlockSize(block_size);

  Kernel->Execute(  symbs_in,
                    symbs_ref,
                    error_tags,
                    block_size );

  return(_MES_AOK);
}

