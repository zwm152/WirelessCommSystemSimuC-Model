//
//  File = mpskoptimdem.cpp
//

#include <stdlib.h>
#include "parmfile.h"
#include "mpskoptimdem.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
using namespace std;

//======================================================

MpskOptimalDemod::MpskOptimalDemod( 
                     char* instance_name,
                     PracSimModel* outer_model,
                     Signal< complex< float > >* in_sig,
                     Signal< bit_t >* symb_clock_in,
                     Signal< byte_t >* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
   MODEL_NAME(MpskOptimalDemod);

   ENABLE_MULTIRATE;

   //  Read model config parms
   OPEN_PARM_BLOCK;
   GET_INT_PARM(Bits_Per_Symb);
   GET_INT_PARM(Samps_Per_Symb);

   //  Connect input and output signals
   Out_Sig = out_sig;
   Symb_Clock_In = symb_clock_in;
   In_Sig = in_sig;

   MAKE_OUTPUT( Out_Sig );
   MAKE_INPUT( Symb_Clock_In );
   MAKE_INPUT( In_Sig );

   double resamp_rate = 1.0/double(Samps_Per_Symb);
   CHANGE_RATE( In_Sig, Out_Sig, resamp_rate );
   CHANGE_RATE( Symb_Clock_In, Out_Sig, resamp_rate );
   Num_Diff_Symbs = 1;
   for(int i=1; i<=Bits_Per_Symb; i++)
      Num_Diff_Symbs *=2;

}
//======================================================
MpskOptimalDemod::~MpskOptimalDemod( void ){ };

//======================================================
void MpskOptimalDemod::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Out_Samp_Intvl = Out_Sig->GetSampIntvl();
  //
  //  set up table of phase references
  Conj_Ref = new std::complex<float>[Num_Diff_Symbs];
  Integ_Val = new double[Num_Diff_Symbs];

  for( byte_t isymb=0; isymb<Num_Diff_Symbs; isymb++){
    Integ_Val[isymb] = 0.0;
    Conj_Ref[isymb] = std::complex<float>( 
       float(cos(TWO_PI * 
       isymb/double(Num_Diff_Symbs))),
       -float(sin(TWO_PI * 
       isymb/double(Num_Diff_Symbs))));
    }
}
//======================================================
int MpskOptimalDemod::Execute()
{
   byte_t *out_sig_ptr;
   std::complex<float> *in_sig_ptr;
   bit_t *symb_clock_in_ptr;
   std::complex<float> in_val;
   double *integ_val;
   double max_val=0.0;
   int is;
   int block_size;
   byte_t isymb, symb_decis;
#ifdef _DEBUG
   *DebugFile << "In MpskOptimalDemod::Execute\0" 
              << endl;
#endif

   in_sig_ptr = GET_INPUT_PTR( In_Sig );
   symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );
   out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );

   block_size = In_Sig->GetValidBlockSize();
   Out_Sig->SetValidBlockSize(block_size/
                              Samps_Per_Symb);

   integ_val = Integ_Val;

   for (is=0; is<block_size; is++){
      in_val = *in_sig_ptr++;

      // correlate input signal against all possible
      // reference phases
      for( isymb=0; isymb<Num_Diff_Symbs; isymb++){
         Integ_Val[isymb] += std::real(in_val * 
                              Conj_Ref[isymb]);
      }

      if(*symb_clock_in_ptr != 0){
         // time to make a decision
         max_val = Integ_Val[0];
         symb_decis = 0;
         for(isymb=1; isymb<Num_Diff_Symbs; isymb++){
            if(Integ_Val[isymb] > max_val){
               max_val = Integ_Val[isymb];
               symb_decis = isymb;
            }
         }
         *out_sig_ptr = symb_decis;
         out_sig_ptr++;

         for(isymb=0; isymb<Num_Diff_Symbs; isymb++){
            Integ_Val[isymb] = 0.0;
         }
      }
      symb_clock_in_ptr++;
   }
   return(_MES_AOK);
}

