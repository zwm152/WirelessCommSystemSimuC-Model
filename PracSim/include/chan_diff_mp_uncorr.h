//
//  File = chan_diff_mp_uncorr.h
//

#ifndef _CHAN_DIFF_MP_UNCORR_H_
#define _CHAN_DIFF_MP_UNCORR_H_

#include "psmodel.h"
#include "signal_T.h"
#include "butt_filt_iir.h"
//#include "filtfunc.h"
//#include "buttfunc.h"
//#include "filt_imp.h"
//#include "cmpx_pm.h"

class ChanDiffuseMultipathUncorr : public PracSimModel
{
public:
  ChanDiffuseMultipathUncorr( char* instance_name,
            PracSimModel* outer_model,
            Signal<std::complex<float> >* in_sig,
            Signal<std::complex<float> >* out_sig);


  ~ChanDiffuseMultipathUncorr(void);
  void Setup(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  double Samp_Intvl;
  //int Out_Block_Size;
  Signal<std::complex<float> > *In_Sig;
  Signal<std::complex<float> > *Out_Sig;
  std::complex<float>* In_Buf;
  ButterworthFilterByIir<float>** Gain_Filter;
  int Num_Taps;
  long Seed;
  long Initial_Seed;
  double *Multipath_Profile;

};

#endif
