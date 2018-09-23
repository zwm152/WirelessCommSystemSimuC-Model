//
//  File = psmodel.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "parmfile.h"
#include "psmodel.h"
#include "syst_graph.h"
#include "model_graph.h"
extern ParmFile ParmInput;
extern ofstream *DebugFile;
extern PracSimModel *PrevModelConstr;
extern SystemGraph CommSystemGraph;
extern int EnclaveNumber;
extern PracSimModel *CommSystem;

//======================================================

PracSimModel::PracSimModel( char* instance_name,
                            PracSimModel* outer_model)
{
   //---------------------------------------------------
   //  Closeout the CMSG for previous model instance
   //  and merge it with the Active System Graph
   Nest_Depth = 1 + outer_model->GetNestDepth();
   if( (PrevModelConstr !=NULL) && (Nest_Depth==1))
   {
#ifdef _DEBUG
      *DebugFile << "model " << instance_name 
         << " is calling CloseoutModelGraph" 
         << endl;
#endif
      PrevModelConstr->CloseoutModelGraph(1);
   }
   if(Nest_Depth == 1) {
      PrevModelConstr = this;
   }
   //--------------------------------------------
   // done with previous instance, now work on
   // the current instance
   MODEL_NAME(unknown);
   Instance_Name = new char[strlen(instance_name)+2];
   strcpy(Instance_Name, instance_name);
   Input_Sigs = NULL;
   //-----------------------------
   //  Register model
   if(Nest_Depth == 1){
      CommSystemGraph.RegisterModel(this);
   }
   //------------------------
   //  create CMSG
   if(Nest_Depth == 1){
      Curr_Mod_Graph = new ModelGraph(this);
   }
}
//======================================================
// special constructor for the CommSystem pseudomodel
//------------------------------------------------------
PracSimModel::PracSimModel(int sys_key,char* model_name)
{
   //-------------------------------------------
   // for special case of CommSystem, model name 
   // and instance name are the same

   Model_Name = new char[strlen(model_name)+2];
   strcpy(Model_Name, model_name);
   Instance_Name = new char[strlen(model_name)+2];
   strcpy(Instance_Name, model_name);

   //  Output_Sigs = NULL;
   Input_Sigs = NULL;
   Nest_Depth = 0;
}
//======================================================
PracSimModel::~PracSimModel()
{
};
//======================================================
void PracSimModel::CloseoutModelGraph(int key)
{
   Curr_Mod_Graph->Closeout( this );
   *DebugFile << "In CloseoutModelGraph ("
              << GetModelName() << "), key = " << key 
              <<", Nest_Depth = " << Nest_Depth << endl;
   if(Nest_Depth == 1){
      CommSystemGraph.MergeCurrModelGraph(
                                    Curr_Mod_Graph);
      CommSystemGraph.DumpSDGraph();
   }
}
//======================================================
const char* PracSimModel::GetModelName(void)
{
   return(Model_Name);
}
//======================================================
const char* PracSimModel::GetInstanceName(void)
{
   return(Instance_Name);
}
//======================================================
int PracSimModel::GetNestDepth(void)
{
  return(Nest_Depth);
}
