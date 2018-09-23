//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = cheb_filt_intg.cpp
//
//  Chebyshev Filter
//

//#include <math.h>
//#include "misdefs.h"
#include "parmfile.h"
#include "cheb_filt_intg.h"
#include "chebyshev_proto.h"
//#include "filter_types.h"
extern ParmFile *ParmInput;

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================
//  constructor

ChebyshevFilterByInteg::ChebyshevFilterByInteg( char *instance_name,
                                  PracSimModel *outer_model,
                                  Signal<float> *in_sig,
                                  Signal<float> *out_sig )
                  :AnalogFilterByInteg( instance_name,
                               outer_model,
                               in_sig,
                               out_sig )
{
  OPEN_PARM_BLOCK;
  // Most of the parameters needed to specify a Chebyshev filter
  // are common to all the classic types and are therefore
  // read by the AnalogFilter base class.
  //
  // 'Ripple' is specific to Chebyshev so it is read here in the drerived class
  

  if( !Bypass_Enabled)
    {
    GET_DOUBLE_PARM(Passband_Ripple_In_Db);
    GET_BOOL_PARM(Bw_Is_Ripple_Bw);
    // construct a Chebyshev prototype
    Lowpass_Proto_Filt = new ChebyshevPrototype(  Prototype_Order, 
                                                  Passband_Ripple_In_Db,
                                                  Bw_Is_Ripple_Bw);
    Lowpass_Proto_Filt->DumpBiquads(DebugFile);
    Lowpass_Proto_Filt->GetDenomPoly(false);
    }
  return;
}
