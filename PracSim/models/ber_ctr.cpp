//
//  File = ber_ctr.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "ber_ctr.h"
#include "typedefs.h"
#include "model_graph.h"
#include "syst_graph.h"
extern ParmFile *ParmInput;
extern ofstream *DebugFile;
extern SystemGraph CommSystemGraph;
extern int PassNumber;
extern int MaxPassNumber;
using namespace std;

//======================================================

BerCounter::BerCounter( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<bit_t>* ref_signal,
                        Signal<bit_t>* in_signal )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(BerCounter);
  In_Sig = in_signal;
  Ref_Sig = ref_signal;

  OPEN_PARM_BLOCK;
  GET_INT_PARM(Num_Holdoff_Passes);
  GET_INT_PARM(Report_Intvl_In_Blocks);

  MAKE_INPUT(In_Sig);
  MAKE_INPUT(Ref_Sig);

  Symb_Count = 0;
  Error_Count = 0;

  char sub_name[50];
  strcpy(sub_name, GetInstanceName());
  strcat(sub_name, ":Kernel\0");
  Kernel = new k_BerCounter(  sub_name, 
                              Num_Holdoff_Passes,
                              Report_Intvl_In_Blocks );
}
//======================================================
BerCounter::~BerCounter( void ){ };

//======================================================
void BerCounter::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
};

//======================================================
int BerCounter::Execute()
{
  bit_t *symbs_in;
  bit_t *symbs_ref;

  //if(PassNumber <= 3 ) return(_MES_AOK);

  symbs_in = GET_INPUT_PTR( In_Sig );
  symbs_ref = GET_INPUT_PTR( Ref_Sig );

  Kernel->Execute(  symbs_in,
                    symbs_ref,
                    Block_Size );

  return(_MES_AOK);
}

