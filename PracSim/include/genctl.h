//
//  File = genctl.h
//

#ifndef _GENCTL_H_
#define _GENCTL_H_

#include "vector"
#include "sigstuff.h"
//#include "psmodel.h"
class PracSimModel;

class GenericControl
{
public:
  GenericControl( char* name, PracSimModel* model );
  ~GenericControl(void);
  char* GetName(void);
  GenericControl* GetId();
protected:
  char* Name;
  PracSimModel* Owning_Model;
  GenericControl* Root_Id;
//  std::vector<GenericSignal*> *Connected_Sigs;
  
};

#endif //_GENCTL_H_
