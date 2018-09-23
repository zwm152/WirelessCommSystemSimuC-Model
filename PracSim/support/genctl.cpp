//
//  File = genctl.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "genctl.h"
#include "sigplot.h"
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
extern SignalPlotter SigPlot;
extern int PassNumber;

//===============================================

GenericControl::GenericControl( char* name, PracSimModel* model )
{
  Owning_Model = model;
  Name = new char[strlen(name)+2];
  strcpy(Name, name);
  //Connected_Sigs = new std::vector<GenericSignal*>;
  Root_Id = this;
}
//===============================================
GenericControl::~GenericControl( void )
{
};

#ifdef NOT_DEFINED
//===============================================
InputConnec* GenericSignal::AddConnection(  PracSimModel* model_ptr,
                                            char* name_inside_model )
{
  char* alt_name;
  alt_name = new char[strlen(name_inside_model)+2];
  strcpy(alt_name, name_inside_model);
  InputConnec* input_connec = new InputConnec(name_inside_model);
  Connected_Models->push_back(model_ptr);
  Alternate_Names->push_back(alt_name);
  Input_Connec->push_back(input_connec);
 // Num_Read_Conns++;
  return(input_connec);
};
#endif
//===============================================
char* GenericControl::GetName()
{
return(Name);
}
//===============================================
GenericControl* GenericControl::GetId()
{
return(Root_Id);
}
