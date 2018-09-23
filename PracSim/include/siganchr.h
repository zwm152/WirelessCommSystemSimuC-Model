//
//  File = siganchr.h
//

#ifndef _SIGANCHR_H_
#define _SIGANCHR_H_

#include "signal_T.h"
#include "psmodel.h"
#include "typedefs.h"



class SignalAnchor : public PracSimModel
{
public:
SignalAnchor( char* instance_name,
            PracSimModel* outer_model,
            Signal< std::complex<float> > *signal, //space between > > is mandatory
            double samp_rate,
            int block_size );

SignalAnchor( char* instance_name,
            PracSimModel* outer_model,
            Signal<float> *signal );
SignalAnchor( char* instance_name,
            PracSimModel* outer_model,
            Signal< std::complex<float> > *signal );
SignalAnchor( char* instance_name,
            PracSimModel* outer_model,
            Signal<float> *signal,
            Signal<float> *signal_2,
            double samp_rate,
            int block_size );
SignalAnchor( char* instance_name,
            PracSimModel* outer_model,
            Signal<float> *signal,
            double samp_rate,
            int block_size );

SignalAnchor( char* instance_name,
            PracSimModel* outer_model,
            Signal<int> *signal,
            double samp_rate,
            int block_size );

SignalAnchor( char* instance_name,
            PracSimModel* outer_model,
            Signal<bit_t> *signal,
            double samp_rate,
            int block_size );

SignalAnchor( char* instance_name,
               PracSimModel* outer_model,
               Signal<bit_t>* bit_signal,
               Signal<byte_t>* byte_signal );

SignalAnchor( char* instance_name,
            PracSimModel* outer_model,
            Signal<bit_t>* bit_signal );

SignalAnchor( char* instance_name,
            PracSimModel* outer_model,
            Signal<int> *signal,
            Signal<int> *signal_2,
            double samp_rate,
            int block_size );

  ~SignalAnchor(void);
  void Initialize(void);
  int Execute(void);

private:
  Signal< std::complex<float> >* Complex_Signal;
  Signal<float>* Float_Signal;
  Signal<float>* Float_Signal_2;
  Signal<int>* Int_Signal;
  Signal<bit_t>* Bit_Signal;
  Signal<byte_t>* Byte_Signal;
  Signal<int>* Int_Signal_2;
  
};

#endif
