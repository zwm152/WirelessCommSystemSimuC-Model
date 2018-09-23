//
//  File = cmpx_iq_plot.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "cmpx_iq_plot.h"
#include "complex_io.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
extern char *DateString;
extern bool DateInFileNames;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

CmpxIqPlot::CmpxIqPlot( 
               char* instance_name,
               PracSimModel* outer_model,
               Signal< std::complex< float > >* in_sig,
               Signal< bit_t >* symb_clock_in )
                  :PracSimModel( instance_name,
                                 outer_model)
{
   MODEL_NAME(CmpxIqPlot);

   //-----------------------------------------
   //  Read model config parms

   OPEN_PARM_BLOCK;

   GET_INT_PARM(Hold_Off);
   GET_INT_PARM(Sampling_Offset);

   GET_STRING_PARM(Plot_File_Name);
   if(DateInFileNames){
      strcat(Plot_File_Name,"_");
      strcat(Plot_File_Name, DateString);
   }
   strcat(Plot_File_Name,".txt");

   //--------------------------------------
   //  Connect input and output signals

   Symb_Clock_In = symb_clock_in;
   In_Sig = in_sig;

   MAKE_INPUT( Symb_Clock_In );
   MAKE_INPUT( In_Sig );

   Plot_File = new ofstream(Plot_File_Name, ios::out);
   Processing_Completed = false;
}
//======================================================
CmpxIqPlot::~CmpxIqPlot( void ){ };

//==============================================
void CmpxIqPlot::Initialize(void)
{
   Block_Size = In_Sig->GetBlockSize();
   Sample_Strobe = -1;
}

//======================================================
int CmpxIqPlot::Execute()
{
   std::complex<float> *in_sig_ptr;
   bit_t *symb_clock_in_ptr;
   std::complex<float> in_val;
   int is;
   int block_size;
#ifdef _DEBUG
   *DebugFile << "In CmpxIqPlot::Execute\0" << endl;
#endif

   if(Processing_Completed) return(_MES_AOK);
   if(PassNumber < Hold_Off) return(_MES_AOK);

   in_sig_ptr = GET_INPUT_PTR( In_Sig );
   symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );

   block_size = In_Sig->GetValidBlockSize();

   for (is=0; is<block_size; is++){
      in_val = *in_sig_ptr++;

      if(*symb_clock_in_ptr != 0) {
         Sample_Strobe = Sampling_Offset;
      }
      if(Sample_Strobe == 0) {
         // time to grab a sample
         *Plot_File << in_val << endl;
      }
      Sample_Strobe--;
      symb_clock_in_ptr++;
   }
   return(_MES_AOK);
}

