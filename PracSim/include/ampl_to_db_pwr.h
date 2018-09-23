//
//  File = ampl_to_db_pwr.h
//

#ifndef _AMPL_TO_DB_PWR_H_
#define _AMPL_TO_DB_PWR_H_

#include "psmodel.h"
#include "signal_T.h"

class AmplitudeToDbPower : public PracSimModel
{
public:
  AmplitudeToDbPower( char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<float>* in_sig,
                  Signal<float>* out_sig);

  ~AmplitudeToDbPower(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  Signal<float> *In_Sig;
  Signal<float> *Out_Sig;
};

#endif
