//
//  File = qpsk_err_ctr.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "qpsk_err_ctr.h"
#include "typedefs.h"
#include "model_graph.h"
#include "syst_graph.h"
extern ParmFile *ParmInput;
extern ofstream *DebugFile;
extern SystemGraph CommSystemGraph;
extern int PassNumber;
extern int MaxPassNumber;

//======================================================

QpskErrorCounter::QpskErrorCounter( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<bit_t>* i_ref_signal,
                        Signal<bit_t>* q_ref_signal,
                        Signal<bit_t>* i_in_signal,
                        Signal<bit_t>* q_in_signal )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(QpskErrorCounter);

  I_In_Sig = i_in_signal;
  Q_In_Sig = q_in_signal;
  I_Ref_Sig = i_ref_signal;
  Q_Ref_Sig = q_ref_signal;

  OPEN_PARM_BLOCK;
  GET_INT_PARM(Num_Holdoff_Passes);
  GET_INT_PARM(Report_Intvl_In_Blocks);

  MAKE_INPUT(I_In_Sig);
  MAKE_INPUT(Q_In_Sig);
  MAKE_INPUT(I_Ref_Sig);
  MAKE_INPUT(Q_Ref_Sig);

  Symb_Count = 0;
  Error_Count = 0;

  char sub_name[50];
  strcpy(sub_name, GetInstanceName());
  strcat(sub_name, ":Kernel\0");
//  Kernel = new k_SerCounter(  sub_name, 
//                              Num_Holdoff_Passes,
//                              Report_Intvl_In_Blocks );
}
//======================================================
QpskErrorCounter::~QpskErrorCounter( void ){ };

//======================================================
void QpskErrorCounter::Initialize(void)
{
  Block_Size = I_In_Sig->GetBlockSize();
};

//======================================================
int QpskErrorCounter::Execute()
{
  bit_t *i_symbs_in, *q_symbs_in;
  bit_t *i_symbs_ref, *q_symbs_ref;
  bit_t i_symb_val, q_symb_val;
  bit_t i_ref_val, q_ref_val;
  int symb_count, error_count;
  int is;

  //if(PassNumber <= 3 ) return(_MES_AOK);

  i_symbs_in = GET_INPUT_PTR( I_In_Sig );
  q_symbs_in = GET_INPUT_PTR( Q_In_Sig );
  i_symbs_ref = GET_INPUT_PTR( I_Ref_Sig );
  q_symbs_ref = GET_INPUT_PTR( Q_Ref_Sig );

  symb_count = Symb_Count;
  error_count = Error_Count;
  bool error_found = false;

  for (is=0; is < Block_Size; is++)
    {
    i_symb_val = *i_symbs_in++;
    q_symb_val = *q_symbs_in++;
    i_ref_val = *i_symbs_ref++;
    q_ref_val = *q_symbs_ref++;
    symb_count++;
    if( (i_symb_val != i_ref_val) || (q_symb_val != q_ref_val) )
      {
      error_found = true;
      error_count++;
      }
    }

  if( ((PassNumber - Num_Holdoff_Passes) % Report_Intvl_In_Blocks) == 0)
    {
    BasicResults << GetModelName() << " ( " << GetInstanceName() << " ): "
              << PassNumber << "  SER = " << (float(error_count)/float(symb_count)) << " -- "
              << error_count << " errors in " << symb_count << " symbols" << endl;
    }
  Symb_Count = symb_count;
  Error_Count = error_count;
  return(_MES_AOK);
}

