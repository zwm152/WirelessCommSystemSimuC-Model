//
//  File = signal_T.h
//

#ifndef _SIGNAL_T_H_
#define _SIGNAL_T_H_
using namespace std;
#include "gensig.h"
#include "typedefs.h"
#include <complex>

template< class T >
class Signal : public GenericSignal
{
public:
//  Signal<T>( char* name, PracSimModel* model );
  Signal<T>( Signal<T>* root_id, char* name, PracSimModel* model );
  Signal<T>( char* name );
  ~Signal<T>(void);
  void AllocateSignalBuffer(void);
  void InitializeReadPtrs(void);
  T* GetRawOutputPtr(PracSimModel* model);
  T* GetRawInputPtr(PracSimModel* model);
  Signal<T>* AddConnection(  PracSimModel* model,
                               char* name_in_model );
//  Signal<T>* AddConnection(  PracSimModel* model,
//                             char* name_in_model,
//                             Signal<T>* new_sig );
  void Dump(ofstream);
  void PassUpdate(void);
  void SetupPlotSignal(void);
  void IssuePlotterData(void);
private:
  T *Buf_Beg;
  T *Phys_Buf_Beg;
  T *Buf_Final_Mem_Beg;
  T *Next_Loc_To_Plot;
  
};

#endif //_SIGNAL_T_H_
