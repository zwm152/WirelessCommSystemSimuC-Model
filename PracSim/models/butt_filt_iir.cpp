//======================================================
//
//  File = butt_filt_iir.cpp
//
//  Butterworth Filter by IIR model
//

#include "butt_filt_iir.h"
#include "butterworth_proto.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================
//  constructor

template < class T>
ButterworthFilterByIir<T>::
      ButterworthFilterByIir( char *instance_name,
                              PracSimModel *outer_model,
                              Signal<T> *in_sig,
                              Signal<T> *out_sig )
                  :AnalogFilterByIir<T>(  instance_name,
                                          outer_model,
                                          in_sig,
                                          out_sig )
{
   MODEL_NAME(ButterworthFilterByIir);
   //OPEN_PARM_BLOCK;
   // All of the parameters needed to specify a 
   // Butterworth filter are common to all the classic
   // types: Chebyshev, Elliptical, Bessel.  Since they
   // will be needed by all types of filters they are
   // read by the AnalogFilter base class.


   if( !Bypass_Enabled)
   {
      // construct a Butterworth prototype
      Lowpass_Proto_Filt = 
            new ButterworthPrototype(Prototype_Order);
      Lowpass_Proto_Filt->FilterFrequencyResponse();
      Lowpass_Proto_Filt->GetDenomPoly(false);
   }
   return;
}
//======================================================
//  constructor for single-sample subordinate version

template < class T>
ButterworthFilterByIir<T>::
         ButterworthFilterByIir( 
                           char *instance_name,
                           PracSimModel *outer_model )
                  :AnalogFilterByIir<T>( 
                              instance_name,
                              outer_model )
{
   MODEL_NAME(ButterworthFilterByIir);

   if( !Bypass_Enabled)
   {
      // construct a Butterworth prototype
      Lowpass_Proto_Filt = 
            new ButterworthPrototype(Prototype_Order);
      Lowpass_Proto_Filt->FilterFrequencyResponse();
      Lowpass_Proto_Filt->GetDenomPoly(false);
   }
   return;
}
//======================================================
//  destructor

template <class T>
ButterworthFilterByIir<T>::~ButterworthFilterByIir(){};
template ButterworthFilterByIir<float>;
template ButterworthFilterByIir<std::complex<float> >;
