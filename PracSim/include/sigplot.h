//
//  File = sigplot.h
//

#ifndef _SIGPLOT_H_
#define _SIGPLOT_H_

#include "gensig.h"

class SignalPlotter
{
public:
  SignalPlotter(void);
  
  ~SignalPlotter(void);
  
  void Initialize(void);

  void SetWakeAndSleep( GenericSignal* signal_to_plot,
                        int wakeup,
                        int bedtime );

  void CollectData(void);
private:
  char *Model_Name;
  int Num_Plot_Sigs;
  int *Wakeup_Pass;
  int *Bedtime_Pass;
  std::vector<GenericSignal*> *Plot_Sig_Id;
  
};

#endif //_SIGPLOT_H_
