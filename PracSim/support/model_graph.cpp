//
//  File = model_graph.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "parmfile.h"
#include "model_graph.h"
extern ParmFile ParmInput;
extern ofstream *DebugFile;

//========================================================================

ModelGraph::ModelGraph(PracSimModel* owning_model)
{
   Max_Signals = 50;
   Owning_Model = owning_model;

   Model_Is_Multirate = false;
   Model_Is_Constant_Interval = false;

   Delta_Delay = new std::vector<double>;
   Vertex_Is_Input = new std::vector<bool>;
   Vertex_Kind = new std::vector<SignalKinds_type>;
   Node_Is_Feedback = new std::vector<bool>;
   Block_Size = new std::vector<int>;
   Samp_Intvl = new std::vector<double>;
   Resamp_Rate = new std::vector<double>;
   Const_Intvl = new std::vector<bool>;
   Sig_Dep_Graph = new DirectedGraph;

   return;
}
//=========================================================================
ModelGraph::~ModelGraph()
{
   delete Vertex_Is_Input;
   delete Vertex_Kind;
};
//=========================================================================
void ModelGraph::InsertSignal( GenericSignal *new_sig_id,
                               PracSimModel *model_ptr,
                               bool new_sig_is_input)
{
   int new_sig_num;
   //--------------------------------------------------------------
   //  Add new vertex into Signal Dependency CMSG

   new_sig_num = Sig_Dep_Graph->AddVertex(new_sig_id);

   //---------------------------------------------
   //  set node properies to default values
   //
   Vertex_Is_Input->push_back(new_sig_is_input);
   Vertex_Kind->push_back(SK_REGULAR_SIGNAL);
   Node_Is_Feedback->push_back(false);
   Block_Size->push_back(0);
   Samp_Intvl->push_back(0.0);

   //------------------------------------------------------------
   //  Loop thru existing vertices to see where edges need to be
   //  added to or from new vertex
   //
   for(int old_vtx=0; old_vtx<Sig_Dep_Graph->GetNumVerts(); old_vtx++) {
      //----------------------------------------------
      //  Don't create edges between new signal and
      //  existing signals of same input/output sense
      //  (i.e. don't make edges from input to input
      //  or from output to output)
      if(Vertex_Is_Input->at(old_vtx) != new_sig_is_input) {
          
         //----------------------------------------------
         //  add edge from input vertex to output vertex

         if(new_sig_is_input){
            // new vertex is INPUT: add edge from NEW vertex
            // to EXISTING vertex
            Sig_Dep_Graph->AddEdge( model_ptr,
                                    new_sig_num,
                                    old_vtx);
         }
         else {
            // new vertex is OUTPUT: add edge from EXISTING
            // vertex to NEW vertex
            Sig_Dep_Graph->AddEdge( model_ptr,
                                    old_vtx,
                                    new_sig_num);
         }

         //--------------------------------------
         // Set edge properties to default values
         //         
         Delta_Delay->push_back(0.0); // only used by a very few models

         //-------------------------------------------------
         //  Make Resampling_Rate undefined if model is a
         //  multirate model or if the existing vertex
         //  just connected to the new edge is a control
         //  rather than a signal

         if( Model_Is_Multirate ){
            Resamp_Rate->push_back(UNDEFINED_RATE);
         }
         else{
            Resamp_Rate->push_back(1.0);
         }

         Const_Intvl->push_back(Model_Is_Constant_Interval);

      } //end of if
   }// end of loop over old_vtx
   return;
}
//====================================================
GenericSignal* ModelGraph::GetSignalId(int vtx_num)
{
   GenericSignal* sig_id;
   sig_id = (GenericSignal*)Sig_Dep_Graph->GetVertexId(vtx_num);
   return(sig_id);
}
//====================================================
void* ModelGraph::GetVertexId(int vtx_num)
{
   void* vtx_id;
   vtx_id = Sig_Dep_Graph->GetVertexId(vtx_num);
   return(vtx_id);
}
//====================================================
void* ModelGraph::GetEdgeId(int edge_num)
{
   void* vtx_id;
   vtx_id = Sig_Dep_Graph->GetEdgeId(edge_num);
   return(vtx_id);
}
//====================================================
PracSimModel* ModelGraph::GetModelId(int edge_num)
{
   PracSimModel* model_id;
   model_id = (PracSimModel*)Sig_Dep_Graph->GetEdgeId(edge_num);
   return(model_id);
}
//====================================================
void ModelGraph::EnableMultirate()
{
   Model_Is_Multirate = true;
   return;
}
//====================================================
void ModelGraph::EnableConstantInterval()
{
   Model_Is_Constant_Interval = true;
   return;
}
//===============================================================
int ModelGraph::GetBlockSize(int vtx_num)
{
   return(Block_Size->at(vtx_num));
}
//===============================================================
int ModelGraph::GetNumVerts(void)
{
   return(Sig_Dep_Graph->GetNumVerts());
}
//===============================================================
void ModelGraph::SetSampIntvl( GenericSignal* sig_id,
                               double samp_intvl)
{
   int sig_num = Sig_Dep_Graph->GetVertexNum(sig_id->GetId());
   Samp_Intvl->at(sig_num) = samp_intvl;
}
//===============================================================
void ModelGraph::SetBlockSize( GenericSignal* sig_id,
                               int block_size)
{
   int sig_num = Sig_Dep_Graph->GetVertexNum(sig_id->GetId());
   Block_Size->at(sig_num) = block_size;
}
//===============================================================
double ModelGraph::GetSampIntvl(int vtx_num)
{
   return(Samp_Intvl->at(vtx_num));
}
//===============================================================
double ModelGraph::GetResampRate(int edge_num)
{
   return(Resamp_Rate->at(edge_num));
}
//===============================================================
bool ModelGraph::GetIsConstIntvl(int edge_num)
{
   return(Const_Intvl->at(edge_num));
}
//===============================================================
double ModelGraph::GetDelay(int edge_num)
{
   return(Delta_Delay->at(edge_num));
}
//===============================================================
bool ModelGraph::ConnIsFeedback(int vtx_num)
{
   return(false);
}
//===============================================================
SignalKinds_type ModelGraph::GetVertexKind(int vtx_num)
{
   return(Vertex_Kind->at(vtx_num));
}
//===========================================================
int ModelGraph::GetEdgeNum( int in_sig_num,
                            int out_sig_num )
{
   int edge_num;
   edge_num = Sig_Dep_Graph->GetEdgeNum(in_sig_num, out_sig_num);
   return(edge_num);
}
//===========================================================
void ModelGraph::ChangeRate( GenericSignal* in_sig_id,
                             GenericSignal* out_sig_id,
                             double resamp_rate,
                             PracSimModel* model_ptr)
{
   int in_sig_num, out_sig_num, edge_num;

   in_sig_num = Sig_Dep_Graph->GetVertexNum(in_sig_id);
   out_sig_num = Sig_Dep_Graph->GetVertexNum(out_sig_id);
   edge_num = Sig_Dep_Graph->GetEdgeNum(in_sig_num, out_sig_num);

   if(edge_num < 0) {
      //---------------------------------
      // -- ERROR -- 
      // could get this error if we try to set a rate change
      // between two inputs or between two outputs -- these 
      // combinations won't have an edge defined between them.

      //ErrorStream << "Error in ModelGraph::ChangeRate -- "
      *DebugFile << "Error in ModelGraph::ChangeRate -- "
                 << "attempt to change rate on undefined edge"
                 << endl;
      exit(1);
   }
   Resamp_Rate->at(edge_num) = resamp_rate;
   return;
}
//====================================================
void ModelGraph::Closeout( PracSimModel* model_ptr)
{
   //-------------------------------------------------------
   //  Each CMG must have at least one input signal and one
   //  output signal.  Check to see if signals in CMG are all
   //  inputs or all outputs.  If they are, augment CMG
   //  by adding a dummy source or dummy destination signal
   //  as appropriate.  (Only signals are checked -- vertices
   //  that represent controls are ignored.)

   //-----------------------------------------------
   // Find first vertex that represents a signal
   // and determine if it is an input or an output.

   int vtx_num, first_sig_idx;
   bool first_is_input;
   bool sigs_have_same_in_out_sense;
   int new_sig_num;
   GenericSignal *dummy_signal;

   int num_verts = Sig_Dep_Graph->GetNumVerts();
   if(num_verts == 0) return;
   first_sig_idx = 0;
   first_is_input = Vertex_Is_Input->at(first_sig_idx);

   //-------------------------------------------------
   // Compare first signal against all other signals

   sigs_have_same_in_out_sense = true;
   for( vtx_num = first_sig_idx+1; vtx_num < num_verts; vtx_num++) {
      if(first_is_input != Vertex_Is_Input->at(vtx_num))
         sigs_have_same_in_out_sense = false;
   }

   if(sigs_have_same_in_out_sense) {
      if(first_is_input) {
         //  all existing signals are inputs: augment
         //  CMG with a dummy destination signal

         Vertex_Is_Input->push_back(false);
         dummy_signal = new GenericSignal( "Dummy_Destination\0",
                                           model_ptr);
         new_sig_num = Sig_Dep_Graph->AddVertex( dummy_signal );
         num_verts++;

         Vertex_Kind->push_back(SK_DUMMY_DEST_SIGNAL);
         Block_Size->push_back(0);
         Samp_Intvl->push_back(0.0);

         //-------------------------------------------------------
         //  add edges from each input signal to added destination

         for( vtx_num = 0; vtx_num < new_sig_num; vtx_num++){
            if(Vertex_Is_Input->at(vtx_num) ){
               Sig_Dep_Graph->AddEdge(model_ptr, vtx_num, new_sig_num);
               Resamp_Rate->push_back(UNDEFINED_RATE);
               Delta_Delay->push_back(0.0);

               if(Model_Is_Constant_Interval){
                  Const_Intvl->push_back(true);
               }
               else {
                  Const_Intvl->push_back(false);
               }
            }
         }
      } // end of if(first_is_input)
      else {
         //--------------------------------------------
         // all existing signals are outputs: augment
         // CMSG with a dummy source signal

         Vertex_Is_Input->push_back(true);
         dummy_signal = new GenericSignal( "Dummy_Source\0",
                                           model_ptr);

         new_sig_num = Sig_Dep_Graph->AddVertex( dummy_signal );
         num_verts++;

         Vertex_Kind->push_back(SK_DUMMY_SOURCE_SIGNAL);
         Block_Size->push_back(0);
         Samp_Intvl->push_back(0.0);

         //-------------------------------------------------------
         //  add edges from added source to each output signal

         for( vtx_num = 0; vtx_num < new_sig_num; vtx_num++){
            if( ! Vertex_Is_Input->at(vtx_num) ){
               Sig_Dep_Graph->AddEdge(model_ptr, new_sig_num, vtx_num);
               Resamp_Rate->push_back(UNDEFINED_RATE);
               Delta_Delay->push_back(0.0);

               if(Model_Is_Constant_Interval){
                  Const_Intvl->push_back(true);
               }
               else {
                  Const_Intvl->push_back(false);
               }
            }
         } // end of loop over vtx_num
      } // end of else clause on if(first_is_input)
   }

   DumpSigDepGraph();

   return;
}
//====================================================
void ModelGraph::DumpSigDepGraph(void)
{
   int sig_idx, sig_idx2;
   int edge_idx;

   cout << "\nIn ModelGraph::DumpSigDepGraph" << endl;
   *DebugFile << "\nIn ModelGraph::DumpSigDepGraph" << endl;

   int num_verts = Sig_Dep_Graph->GetNumVerts();
   *DebugFile << "\nVertices: " << num_verts << endl;

   for( sig_idx = 0; sig_idx < num_verts; sig_idx++) {
      *DebugFile << Sig_Dep_Graph->GetVertexId(sig_idx) << "  "
                 << ((GenericSignal*)Sig_Dep_Graph->GetVertexId(sig_idx))->GetName()
                 << endl;
   }

   int num_edges = Sig_Dep_Graph->GetNumEdges();
   *DebugFile << "\nEdges: " << num_edges << endl;

   for( edge_idx = 0; edge_idx < num_edges; edge_idx++){
      *DebugFile << Sig_Dep_Graph->GetEdgeId(edge_idx) << "  "
                 << ((PracSimModel*)Sig_Dep_Graph->GetEdgeId(edge_idx))->GetModelName()
                 << endl;
   }

   *DebugFile << "\nAdjacency Matrix:" << endl;
   for( sig_idx = 0; sig_idx < num_verts; sig_idx++){

      for( sig_idx2 = 0; sig_idx2 < num_verts; sig_idx2++) {
         *DebugFile << Sig_Dep_Graph->GetEdgeNum(sig_idx, sig_idx2)
                    << "   ";
      }
      *DebugFile << endl;
   }
   return;
}
//==================================================================
PracSimModel* ModelGraph::GetOwningModel(void)
{
   return(Owning_Model);
}
