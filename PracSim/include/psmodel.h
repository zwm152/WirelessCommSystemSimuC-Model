//
//  File = psmodel.h
//
#ifndef _PSMODEL_H_
#define _PSMODEL_H_

#include "gensig.h"
#include "list"
class ModelGraph;

#define _MES_AOK 0
#define _MES_RESTART 1
#define MODEL_NAME(X) { \
Model_Name = strcpy(new char[strlen(#X)+2], #X);}
//======================================================
class PracSimModel
{
public:
   PracSimModel( char *model_name, 
                 PracSimModel* outer_model);
   PracSimModel( int dummy_for_unique_signature,
      char *model_name);
   ~PracSimModel(void);
   const char* GetModelName(void);
   const char* GetInstanceName(void);
   int GetNestDepth(void);
   void CloseoutModelGraph(int key);
   virtual void Initialize(void){};
   virtual int Execute(void){return(-1);};
protected:
   typedef struct{
      GenericSignal *Ptr_To_Sig;
      bool Sig_Is_Optional;
   } Sig_List_Elem;

   char *Model_Name;
   char *Instance_Name;
   std::list<Sig_List_Elem*> *Output_Sigs;
   std::list<Sig_List_Elem*> *Input_Sigs;
   ModelGraph* Curr_Mod_Graph;
   int Nest_Depth;
};
#endif // _PSMODEL_H_