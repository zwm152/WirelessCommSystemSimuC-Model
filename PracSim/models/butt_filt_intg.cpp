//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = butt_filt_intg.cpp
//
//  Butterworth Filter by IIR model
//

//#include <math.h>
//#include "misdefs.h"
//#include "parmfile.h"
#include "butt_filt_intg.h"
#include "butterworth_proto.h"
//#include "filter_types.h"
//extern ParmFile ParmInput;

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//================================================================
//  constructor

ButterworthFilterByInteg::ButterworthFilterByInteg( char *instance_name,
                                      PracSimModel *outer_model,
                                      Signal<float> *in_sig,
                                      Signal<float> *out_sig )
                  :AnalogFilterByInteg( instance_name,
                               outer_model,
                               in_sig,
                               out_sig )
{
  //OPEN_PARM_BLOCK;
  // All of the parameters needed to specify a Butterworth filter
  // are common to all the classic types: Chebyshev, Elliptical, Bessel.
  // Since they will be needed by all types of filters they are
  // read by the AnalogFilter base class.
  

  //if( !Bypass_Enabled)
    //{
    // construct a Butterworth prototype
    Lowpass_Proto_Filt = new ButterworthPrototype(Prototype_Order);
    Lowpass_Proto_Filt->DumpBiquads(DebugFile);
    Lowpass_Proto_Filt->GetDenomPoly(false);
    //}
  return;
}
