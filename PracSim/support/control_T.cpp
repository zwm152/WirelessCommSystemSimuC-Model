//
//  File = control_T.cpp
//

#include <stdlib.h>
#include <fstream>
#include <ostream>
#include <iomanip>
#include "control_T.h"
#include "typedefs.h"
#include "complex_io.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
extern PracSimModel* CommSystem;
extern int PassNumber;

//======================================================

template< class T >
Control<T>::Control<T>( char* name, PracSimModel* model )
          :GenericControl( name, model )
{
  Cntrl_Value = 0;
}
//======================================================

template< class T >
Control<T>::Control<T>( char* name )
          :GenericControl( name, CommSystem )
{
  Cntrl_Value = 0;
}
//======================================================
template< class T >
Control<T>::~Control<T>( void )
{
};

//===============================================
template< class T >
T Control<T>::GetValue(void)
{
  return(Cntrl_Value);
}
//===============================================
template< class T >
void Control<T>::SetValue(T value)
{
  *DebugFile << "setting " << GetName() << " to " << value << endl;
  Cntrl_Value = value;
  return;
}
//===============================================
//template< class T >
//void Control<T>::Dump(ostream out_file)
//{
//  out_file << GetName() << " = " << Cntrl_Value << endl;
//  return;
//}
template Control<bool>;
template Control<int>;
template Control<float>;
template Control<double>;

