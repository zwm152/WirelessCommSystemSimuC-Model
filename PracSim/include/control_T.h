//
//  File = control_T.h
//

#ifndef _CONTROL_T_H_
#define _CONTROL_T_H_

#include "genctl.h"
#include <iostream>
#include <fstream>

template< class T >
class Control : public GenericControl
{
public:
  Control<T>( char* name, PracSimModel *model );
  Control<T>( char* name );
  ~Control<T>(void);
  T GetValue(void);
  void SetValue(T value);
//  void Dump(ofstream);
private:
  T Cntrl_Value;
  
};

#endif //_CONTROL_T_H_
