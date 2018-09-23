//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = bess_filt_iir.cpp
//
//  Bessel Filter by IIR model
//

//#include <math.h>
//#include "misdefs.h"
#include "parmfile.h"
#include "bess_filt_iir.h"
#include "bessel_proto.h"
//#include "filter_types.h"
extern ParmFile *ParmInput;

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================
//  constructor

template < class T>
BesselFilterByIir<T>::BesselFilterByIir( char *instance_name,
                                      PracSimModel *outer_model,
                                      Signal<T> *in_sig,
                                      Signal<T> *out_sig )
                  :AnalogFilterByIir<T>( instance_name,
                               outer_model,
                               in_sig,
                               out_sig )
{
  MODEL_NAME(BesselFilterByIir);
  //OPEN_PARM_BLOCK;
  // All of the parameters needed to specify a Butterworth filter
  // are common to all the classic types: Chebyshev, Elliptical, Bessel.
  // Since they will be needed by all types of filters they are
  // read by the AnalogFilter base class.
  
  if( !Bypass_Enabled)
    {
    GET_BOOL_PARM(Delay_Norm_Enabled);

    // construct a Bessel prototype
    Lowpass_Proto_Filt = new BesselPrototype( Prototype_Order,
                                              Delay_Norm_Enabled);
    Lowpass_Proto_Filt->DumpBiquads(DebugFile);
    Lowpass_Proto_Filt->GetDenomPoly(false);
    }
  return;
};

template <class T>
BesselFilterByIir<T>::~BesselFilterByIir(){};
template BesselFilterByIir<float>;
template BesselFilterByIir<std::complex<float> >;
