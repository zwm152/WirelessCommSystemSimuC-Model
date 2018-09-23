//
//  File = discrete_advance2.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "sigplot.h"
#include "model_error.h"
#include "discrete_adv_T.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern SignalPlotter SigPlot;
extern int PassNumber;
extern int EnclaveNumber;
extern int EnclaveOffset[10];
extern PracSimModel *ActiveModel;
#ifdef _DEBUG
   extern ofstream *DebugFile;
#endif

//======================================================
// general constructor that supports any of the possible 
// advance modes

template< class T >
DiscreteAdvance< T >::DiscreteAdvance( 
                    char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_sig,
                    Signal<T>* out_sig,
                    Control<int>* dynam_adv_cntrl,
                    Control<bool>* adv_chg_enab_cntrl )
              :PracSimModel(instance_name,
                            outer_model)
{
   this->Constructor_Common_Tasks( instance_name, 
                                  in_sig, out_sig);

   //---------------------------------------------------
   // Controls
   Dynam_Adv_Cntrl = dynam_adv_cntrl;
   Adv_Chg_Enab_Cntrl = adv_chg_enab_cntrl;
};
//======================================================
//  Constructor 2
//  This constructor supports any advance mode except 
//  ADVANCE_MODE_GATED (The calling sequence does not 
//  pass the gating control.)

template< class T >
DiscreteAdvance< T >::DiscreteAdvance(
                       char* instance_name,
                       PracSimModel* outer_model,
                       Signal<T>* in_sig,
                       Signal<T>* out_sig,
                       Control<int>* dynam_adv_cntrl )
              :PracSimModel(instance_name,
                            outer_model)
{
   this->Constructor_Common_Tasks( instance_name, 
                                   in_sig, out_sig);

   //---------------------------------------------------
   // Controls
   Dynam_Adv_Cntrl = dynam_adv_cntrl;
   if(Advance_Mode == ADVANCE_MODE_GATED){
      ostrstream temp_stream;
      temp_stream << "ADVANCE_MODE_GATED is not " 
         << "supported by called constructor (2)"
         << ends;
      char *message = temp_stream.str();
      PsModelError(FATAL, message);
      delete []message;
   }
};
//======================================================
//  Constructor 3
//  This constructor supports only ADVANCE_MODE_NONE 
//  and ADVANCE_MODE_FIXED (The calling sequence does 
//  not pass the advance control or the gating control.)

template< class T >
DiscreteAdvance< T >::DiscreteAdvance(
                             char* instance_name,
                             PracSimModel* outer_model,
                             Signal<T>* in_sig,
                             Signal<T>* out_sig )
              :PracSimModel( instance_name,
                             outer_model)
{
   this->Constructor_Common_Tasks( instance_name, 
      in_sig, out_sig);

   //---------------------------------------------------
   // Controls
   if(Advance_Mode == ADVANCE_MODE_DYNAMIC){
      ostrstream temp_stream;
      temp_stream << "ADVANCE_MODE_DYNAMIC is not " 
         << "supported by called constructor (3)"
         << ends;
      char *message = temp_stream.str();
      PsModelError(FATAL, message);
      delete []message;
   }
   if(Advance_Mode == ADVANCE_MODE_GATED){
      ostrstream temp_stream;
      temp_stream << "ADVANCE_MODE_GATED is not " 
         << "supported by called constructor (3)"
         << ends;
      char *message = temp_stream.str();
      PsModelError(FATAL, message);
      delete []message;
   }
};
//======================================================
template< class T >
void DiscreteAdvance< T >
      ::Constructor_Common_Tasks( char *instance_name,
                                  Signal<T> *in_sig,
                                  Signal<T> *out_sig)
{
   MODEL_NAME(DiscreteAdvance);
   ActiveModel = this;
   OPEN_PARM_BLOCK;

   Advance_Mode = GetAdvanceModeParm("Advance_Mode\0");
   BasicResults << "   " << "Advance_Mode = " 
                << Advance_Mode << endl;

   GET_INT_PARM(Initial_Adv_In_Samps);

   switch (Advance_Mode){
   case ADVANCE_MODE_NONE:
   case ADVANCE_MODE_FIXED:
      Max_Adv_In_Samps = Initial_Adv_In_Samps;
      Num_Initial_Passes = 0;
      break;
   case ADVANCE_MODE_DYNAMIC:
   case ADVANCE_MODE_GATED:
      GET_INT_PARM(Max_Adv_In_Samps);
      GET_INT_PARM(Num_Initial_Passes);
      break;
   }

   // Signals
   In_Sig = in_sig;
   Out_Sig = out_sig;

   MAKE_INPUT(In_Sig);
   EnclaveNumber++; // must come after MAKE_INPUT and 
                    // before MAKE_OUTPUT
   MAKE_OUTPUT(Out_Sig);
};

//======================================================
template< class T >
DiscreteAdvance< T >::~DiscreteAdvance( void ){ };

//======================================================
template< class T >
void DiscreteAdvance< T >::Initialize( void )
{
  EnclaveNumber++;
  Block_Size = In_Sig->GetBlockSize();
  Return_Status = _MES_RESTART;
  New_Pass_Number = 0;

  //-----------------------------------------------------
  //  initialize internal buffer

  Blocks_Of_Offset = 
               int(ceil(double(Max_Adv_In_Samps)/
               double(Block_Size)));
  EnclaveOffset[EnclaveNumber]=Blocks_Of_Offset;
  Max_Buffer_Len = Block_Size * Blocks_Of_Offset;
  Start_Of_Buffer = new T[Max_Buffer_Len];
  for(int i=0; i<Max_Buffer_Len; i++)
    {
    *(Start_Of_Buffer+i) = 0;
    }
  End_Of_Buffer = Start_Of_Buffer + Max_Buffer_Len - 1;
  Write_Ptr = Start_Of_Buffer;
  Active_Adv_In_Samps = Initial_Adv_In_Samps;
  Read_Ptr = Start_Of_Buffer + Active_Adv_In_Samps;
  Num_Blocks_Skipped = 0;
};
//======================================================
template< class T >
int DiscreteAdvance< T >::Execute()
{
  T *start_of_buffer, *end_of_buffer;
  T *in_sig_ptr, *out_sig_ptr;
  T *read_ptr, *write_ptr;
  T in_samp, out_samp;
  int is, block_size;

  // collect plotting data for signals in current 
  // block-synchronous enclave before incrementing 
  // to next enclave
  SigPlot.CollectData();
  EnclaveNumber++;
  *DebugFile << "moved to Enclave " << EnclaveNumber << endl;

  //-------------------------------------------------------------
  //  Get pointers for input and output signals

  out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);
  in_sig_ptr = GET_INPUT_PTR(In_Sig);
  block_size = In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(block_size);

  //Remainder_Samps_To_Adv = Total_Samps_Adv % Block_Size;
  //Whole_Blocks_To_Adv = (Total_Samps_Adv - Remainder_Samps_To_Adv) / Block_Size;
  //Buf_Len = (Block_Size - Remainder_Samps_To_Adv);

  //-----------------------------------------------------------
  //  do actions peculiar to each advance mode

  switch (Advance_Mode)
    {
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case ADVANCE_MODE_NONE:
      {
      //...copy input directly to output
      for(is=0; is<Block_Size; is++)
        {
        in_samp = *in_sig_ptr++;
        *out_sig_ptr++ = in_samp;
        }
      return(_MES_AOK);
      }
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case ADVANCE_MODE_FIXED:
      break;
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case ADVANCE_MODE_GATED:
      {
      //  If advance change is NOT enabled, get out of switch.
      //  If delay change is enabled, fall through to next case
      //  and get new value for Active_Advance

      if(Adv_Chg_Enab_Cntrl->GetValue() == false )
        {
        break;
        }
      }
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case ADVANCE_MODE_DYNAMIC:
      {
      if( PassNumber > Num_Initial_Passes)
        {
        Active_Adv_In_Samps = Dynam_Adv_Cntrl->GetValue();
        if(Active_Adv_In_Samps < 0) Active_Adv_In_Samps = 0;
        //Read_Ptr = Write_Ptr - (Max_Adv_In_Samps - Active_Adv_In_Samps);
        Read_Ptr = Write_Ptr - (Max_Buffer_Len - Active_Adv_In_Samps);
        if(Read_Ptr < Start_Of_Buffer) Read_Ptr += Max_Buffer_Len;
        }
      }
    } //end of switch on Advance_mode
  //-----------------------------------------------------------
  //  copy frequently used items into local storage

  //block_size = Block_Size;
  read_ptr = Read_Ptr;
  write_ptr = Write_Ptr;
  start_of_buffer = Start_Of_Buffer;
  end_of_buffer = End_Of_Buffer;

  //------------------------------------------------------


  //----------------------------------------------------------------
  // if the number of blocks already loaded into buffer is less than 
  // the offset between the input enclave and output enclave, then
  // the current input block needs to be loaded into buffer 

  if(Num_Blocks_Skipped < Blocks_Of_Offset)
    {
    for(is=0; is<block_size; is++)
      {
      in_samp = *in_sig_ptr++;
      *write_ptr++ = in_samp;
      }
    if(write_ptr > end_of_buffer) write_ptr = start_of_buffer;
    Num_Blocks_Skipped++;
    }

  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if the number of blocks already loaded into the buffer equals 
  // or exceeds the offset between the input enclave and output enclave,
  // then everything is ready for normal operation

  else
    {
    Return_Status = _MES_AOK;
    for(is=0; is<block_size; is++)
      {
      in_samp = *in_sig_ptr++;
      out_samp = *read_ptr++;
      if(read_ptr > end_of_buffer) read_ptr = start_of_buffer;

      *write_ptr++ = in_samp;
      if(write_ptr > end_of_buffer) write_ptr = start_of_buffer;

      *out_sig_ptr++ = out_samp;
      }
    New_Pass_Number++;
    }
  PassNumber = New_Pass_Number;
  Read_Ptr = read_ptr;
  Write_Ptr = write_ptr;
  return(Return_Status);
}
template DiscreteAdvance< int >;
template DiscreteAdvance< float >;

