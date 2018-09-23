//
//  File = siganchr.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "siganchr.h"
#include "typedefs.h"
#include "model_graph.h"
#include "syst_graph.h"
extern ParmFile *ParmInput;
extern ofstream *DebugFile;
extern SystemGraph CommSystemGraph;
extern int PassNumber;

//======================================================

SignalAnchor::SignalAnchor( char* instance_name,
                        PracSimModel* outer_model,
                        Signal< std::complex<float> >* complex_signal,
                        double samp_intvl,
                        int block_size )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(SignalAnchor);
  Complex_Signal = complex_signal;

  MAKE_INPUT(Complex_Signal);
  SET_SAMP_INTVL(Complex_Signal, samp_intvl);
  SET_BLOCK_SIZE(Complex_Signal, block_size);

}
//====================================================
SignalAnchor::SignalAnchor( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<float>* float_signal )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(SignalAnchor);
  int block_size;
  double samp_intvl;
  
  OPEN_PARM_BLOCK;
  
  GET_DOUBLE_PARM( samp_intvl );
  GET_INT_PARM( block_size );
  Float_Signal = float_signal;

  MAKE_INPUT(Float_Signal);
  SET_SAMP_INTVL(Float_Signal, samp_intvl);
  SET_BLOCK_SIZE(Float_Signal, block_size);

}
//====================================================
SignalAnchor::SignalAnchor( char* instance_name,
                        PracSimModel* outer_model,
                        Signal< std::complex<float> >* complex_signal )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(SignalAnchor);
  int block_size;
  double samp_intvl;
  
  OPEN_PARM_BLOCK;
  
  GET_DOUBLE_PARM( samp_intvl );
  GET_INT_PARM( block_size );
  Complex_Signal = complex_signal;

  MAKE_INPUT(Complex_Signal);
  SET_SAMP_INTVL(Complex_Signal, samp_intvl);
  SET_BLOCK_SIZE(Complex_Signal, block_size);

}
//====================================================
SignalAnchor::SignalAnchor( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<bit_t>* bit_signal )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(SignalAnchor);
  int block_size;
  double samp_intvl;
  
  OPEN_PARM_BLOCK;
  
  GET_DOUBLE_PARM( samp_intvl );
  GET_INT_PARM( block_size );
  Bit_Signal = bit_signal;

  MAKE_INPUT(Bit_Signal);
  SET_SAMP_INTVL(Bit_Signal, samp_intvl);
  SET_BLOCK_SIZE(Bit_Signal, block_size);

}
//====================================================
SignalAnchor::SignalAnchor( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<bit_t>* bit_signal,
                        Signal<byte_t>* byte_signal )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(SignalAnchor);
  int block_size;
  double samp_intvl;
  
  OPEN_PARM_BLOCK;
  
  GET_DOUBLE_PARM( samp_intvl );
  GET_INT_PARM( block_size );
  Bit_Signal = bit_signal;
  Byte_Signal = byte_signal;

  MAKE_INPUT(Bit_Signal);
  MAKE_INPUT(Byte_Signal);
  SET_SAMP_INTVL(Bit_Signal, samp_intvl);
  SET_BLOCK_SIZE(Bit_Signal, block_size);
  SET_SAMP_INTVL(Byte_Signal, samp_intvl);
  SET_BLOCK_SIZE(Byte_Signal, block_size);

}
//====================================================
// float signal
SignalAnchor::SignalAnchor( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<float>* float_signal,
                        double samp_intvl,
                        int block_size )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(SignalAnchor);
  Float_Signal = float_signal;

  MAKE_INPUT(Float_Signal);
  SET_SAMP_INTVL(Float_Signal, samp_intvl);
  SET_BLOCK_SIZE(Float_Signal, block_size);

}
//====================================================
// Dual float signal
SignalAnchor::SignalAnchor( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<float>* float_signal,
                        Signal<float>* float_signal_2,
                        double samp_intvl,
                        int block_size )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(SignalAnchor);
  Float_Signal = float_signal;
  Float_Signal_2 = float_signal_2;

  MAKE_INPUT(Float_Signal);
  MAKE_INPUT(Float_Signal_2);
  SET_SAMP_INTVL(Float_Signal, samp_intvl);
  SET_BLOCK_SIZE(Float_Signal, block_size);
  SET_SAMP_INTVL(Float_Signal_2, samp_intvl);
  SET_BLOCK_SIZE(Float_Signal_2, block_size);

}
//====================================================
SignalAnchor::SignalAnchor( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<int>* int_signal,
                        double samp_intvl,
                        int block_size )
          :PracSimModel(instance_name,
                        outer_model)
{
  MODEL_NAME(SignalAnchor);
  Int_Signal = int_signal;

  MAKE_INPUT(Int_Signal);
  SET_SAMP_INTVL(Int_Signal, samp_intvl);
  SET_BLOCK_SIZE(Int_Signal, block_size);

}
//====================================================
SignalAnchor::SignalAnchor( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<bit_t>* bit_signal,
                        double samp_intvl,
                        int block_size )
          :PracSimModel(instance_name,
                        outer_model)
{
  Bit_Signal = bit_signal;

  MAKE_INPUT(Bit_Signal);
  SET_SAMP_INTVL(Bit_Signal, samp_intvl);
  SET_BLOCK_SIZE(Bit_Signal, block_size);

}
//====================================================
// Dual integer signals
SignalAnchor::SignalAnchor( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<int>* int_signal,
                        Signal<int>* int_signal_2,
                        double samp_intvl,
                        int block_size )
          :PracSimModel(instance_name,
                        outer_model)
{
  Int_Signal = int_signal;
  Int_Signal_2 = int_signal_2;

  MAKE_INPUT(Int_Signal);
  MAKE_INPUT(Int_Signal_2);
  SET_SAMP_INTVL(Int_Signal, samp_intvl);
  SET_BLOCK_SIZE(Int_Signal, block_size);
  SET_SAMP_INTVL(Int_Signal_2, samp_intvl);
  SET_BLOCK_SIZE(Int_Signal_2, block_size);

}
//===================================================
SignalAnchor::~SignalAnchor( void ){ };

void SignalAnchor::Initialize(void)
{
};

int SignalAnchor::Execute()
{
  return(_MES_AOK);
}

