//
//  File = syst_graph.cpp
//

#include <stdlib.h>
#include <fstream>
#include "syst_graph.h"
#include "globals.h"
#include "sigplot.h"
//#include "sig_mgr.h"
//#include "cntl_mgr.h"
//#include "actsysgp.h"
//#include "cmsg.h"
//#include "reinit.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

extern SignalPlotter SigPlot;
//extern Reinitializer Reinit;
extern int PassNumber;
extern int EnclaveNumber;
extern int EnclaveOffset[10];
extern PracSimModel *ActiveModel;

//============================================
// constructor

SystemGraph::SystemGraph()
{
  Sdg_Vert_Descr = new std::vector<sdg_sig_desc_type*>;
  Sdg_Edge_Descr = new std::vector<sdg_edge_desc_type*>;
  Samp_Intvl = new std::vector<double>;
  Block_Size = new std::vector<int>;
  Syst_Lev_Models = new std::vector<PracSimModel*>;
  Sig_Dep_Graph = new DirectedGraph;
  Num_Regular_Sigs = 0;
  return;
}
//============================================
// destructor

SystemGraph::~SystemGraph()
{
}
//============================================

void SystemGraph::MergeCurrModelGraph( ModelGraph* curr_mod_graph )
{
   std::vector<int> merged_verts;
   int num_cmg_verts;
   int cmg_vert_num;
   int sdg_sig_num;
   obj_id_type sdg_sig_id, cmg_sig_id;
   SignalKinds_type cmg_vert_kind;
   bool node_not_found;
   double cmg_samp_intvl;
   int cmg_block_size;

   //------------------------------------------------------------
   // First section of code merges the vertices from the current
   // model graph (CMG) into the system dependency graph (SDG)
   //
   //  Check each signal from CSG to see if it's already in SDG

   num_cmg_verts = curr_mod_graph->GetNumVerts();

   //-----------------------------------------------------
   //  This loop checks each signal from CMG to see if it's 
   //  already in SDG

   for(  cmg_vert_num = 0; 
         cmg_vert_num < num_cmg_verts;
         cmg_vert_num++) {
      cmg_vert_kind = curr_mod_graph->GetVertexKind(cmg_vert_num);
      cmg_sig_id = curr_mod_graph->GetVertexId(cmg_vert_num);

      node_not_found = true;
      int num_sdg_verts = Sig_Dep_Graph->GetNumVerts();

      for( sdg_sig_num = 0; 
           (sdg_sig_num < num_sdg_verts) & node_not_found; 
           sdg_sig_num++) {

         sdg_sig_id = Sig_Dep_Graph->GetVertexId(sdg_sig_num);

         if( sdg_sig_id == cmg_sig_id ) {
            //---------------------------------------
            // this CMSG signal is already in SDG, so add its
            // index (in SDG) to list of merged signals

            merged_verts.push_back(sdg_sig_num);

            //----------------------------------------
            // If sampling rate is undefined for signal in SDG,
            // copy value from CMG signal (which may or may not be 
            // defined).  If sampling rate is defined for signal in SDG,
            // check to see if there is a different value defined in
            // CMG.  If there is, a fatal error condition exists.

            cmg_samp_intvl = curr_mod_graph->GetSampIntvl(cmg_vert_num);

            if(((Sdg_Vert_Descr->at(sdg_sig_num))->samp_intvl) == 0.0) {
               ((Sdg_Vert_Descr->at(sdg_sig_num))->samp_intvl) = cmg_samp_intvl;
            }
            else {
               if( (cmg_samp_intvl != 0.0) &&
                   (cmg_samp_intvl != 
                   ((Sdg_Vert_Descr->at(sdg_sig_num))->samp_intvl)) ) {

                  // -- ERROR --
                  *DebugFile << "Fatal error -- "
                             << "incomsistent sampling rate for signal "
                             << ((GenericSignal*)sdg_sig_id)->GetName() << endl;
                  exit(1);
               }
            }

            //---------------------------------------------------
            //  If block size is undefined for signal in SDG,
            //  copy value from CMG signal (which may or may not be
            //  defined.  If block size is defined for signal in SDG,
            //  check to see if there is a different value in CMG.
            //  If there is, a fatal error condition exists.

            cmg_block_size = curr_mod_graph->GetBlockSize(cmg_vert_num);
            if( ((Sdg_Vert_Descr->at(sdg_sig_num))->block_size) == 0 ) {
               ((Sdg_Vert_Descr->at(sdg_sig_num))->block_size) = cmg_block_size;
            }
            else {
               if( (cmg_block_size != 0 ) &&
                   (cmg_block_size != 
                   ((Sdg_Vert_Descr->at(sdg_sig_num))->block_size) )) {
               // -- ERROR --
               *DebugFile << "Fatal error -- "
                          << "inconsistent block size for signal "
                          << ((GenericSignal*)sdg_sig_id)->GetName() << endl;
               exit(1);
               }
            }
            // no need to check remaining SDG signals for this CMSG signal
            node_not_found = false;

         } // end of if( sdg_sig_id == cmg_sig_id )
      } // end of loop over sdg_sig_num

      //-----------------------------------------------------
      // If loop above completed without a match, we need
      // to add CMG vertex to SDG

      if( node_not_found ) {

         // maybe...we should use the sig_desc_type structure
         // in the CMG, then this would become a simple
         // structure to structure copy
         sdg_sig_desc_type* new_sig_desc = new sdg_sig_desc_type;
         new_sig_desc->signal_id = 
                     curr_mod_graph->GetSignalId(cmg_vert_num);
         new_sig_desc->block_size = 
                     curr_mod_graph->GetBlockSize(cmg_vert_num);
         new_sig_desc->samp_intvl = 
                     curr_mod_graph->GetSampIntvl(cmg_vert_num);
         new_sig_desc->kind_of_signal = 
                     curr_mod_graph->GetVertexKind(cmg_vert_num);

         if(new_sig_desc->kind_of_signal == SK_REGULAR_SIGNAL) Num_Regular_Sigs++;
         Sdg_Vert_Descr->push_back(new_sig_desc);
         sdg_sig_num = Sig_Dep_Graph->AddVertex(cmg_sig_id);
         merged_verts.push_back(sdg_sig_num);
      }
   } // end of loop over cmg_vert_num


   //----------------------------------------------------------
   //  at this point we are done adding vertices, and now begin
   //  to add edges between these vertices as specified in the CMG
   //
   //  new edges in the SDG can only connect to vertices that were
   //  just added or which matched vertices in the CMG

   int sdg_fm_idx, sdg_to_idx;
   int cmg_edge_num;
   int existing_edge;
   bool conn_is_feedback;
   obj_id_type cmg_edge_id;

   int num_merged_verts;
	   num_merged_verts = int(merged_verts.size());
   for(  int cmg_fm_idx = 0; 
         cmg_fm_idx < num_merged_verts; 
         cmg_fm_idx++)  {

      sdg_fm_idx = merged_verts.at(cmg_fm_idx);
      conn_is_feedback = curr_mod_graph->ConnIsFeedback(cmg_fm_idx);

      for( int cmg_to_idx = 0; 
           cmg_to_idx < num_merged_verts; 
           cmg_to_idx++) {

         sdg_to_idx = merged_verts.at(cmg_to_idx);
         cmg_edge_num = curr_mod_graph->GetEdgeNum(cmg_fm_idx, cmg_to_idx);

         if(cmg_edge_num >= 0) {
            //---------------------------------
            // CMG contains an edge from vertex 'cmg_fm_idx'
            // to vertex 'cmg_to_idx'
            // See if ASG-SDG already contains an edge between
            // the corresponding vertices.

            existing_edge = 
                  Sig_Dep_Graph->GetEdgeNum(sdg_fm_idx, sdg_to_idx);

            if(existing_edge >= 0 ) {
               double cmg_resamp = curr_mod_graph->GetResampRate(cmg_edge_num);
              // double sdg_resamp = Sig_Dep_Graph->GetResampRate(existing_edge);
               double sdg_resamp = (Sdg_Edge_Descr->at(existing_edge))->resamp_rate;;

               if(cmg_resamp != sdg_resamp)
               {
                  // parallel edge exists in ASG-SDG -- not sure how to
                  // handle -- see old listing for handling of edges
                  // due to subordinate models
                  *DebugFile << "parallel edge error" << endl;
                  *DebugFile << "existing_edge = " << existing_edge << endl;
                  *DebugFile << "cmg_fm_idx = " << cmg_fm_idx << endl;
                  *DebugFile << "cmg_to_idx = " << cmg_to_idx << endl;
                  exit(1);
               }
            }
            //-----------------------------------------
            // OK to add edge
      
            cmg_edge_id = curr_mod_graph->GetEdgeId(cmg_edge_num);
            Sig_Dep_Graph->AddEdge( cmg_edge_id, 
                                    sdg_fm_idx, 
                                    sdg_to_idx);

            sdg_edge_desc_type* new_edge_desc = new sdg_edge_desc_type;
            new_edge_desc->model_id = (PracSimModel*)cmg_edge_id;
            new_edge_desc->resamp_rate = 
                     curr_mod_graph->GetResampRate(cmg_edge_num);
            new_edge_desc->is_const_intvl = 
                     curr_mod_graph->GetIsConstIntvl(cmg_edge_num);
            new_edge_desc->delta_delay =
                     curr_mod_graph->GetDelay(cmg_edge_num);
            Sdg_Edge_Descr->push_back(new_edge_desc);

         } // end of if(cmg_edge_num)

      } // end of loop over cmg_to_idx

   } // end of loop over cmg_fm_idx
   return;
}
//========================================================
void SystemGraph::DumpSDGraph(void)
{
   int sig_num, sig_num2, edge_num;
   GenericSignal *sig_id;
   PracSimModel *model_id;
   int num_nodes = Sig_Dep_Graph->GetNumVerts();
   int num_edges = Sig_Dep_Graph->GetNumEdges();

   *DebugFile << "\nIn SystemGraph::DumpSDGraph" << endl;
   *DebugFile << "Num_SDG_Nodes = " << num_nodes << endl;
   *DebugFile << "Vertices:" << endl;

   *DebugFile << "index, Sdg_Vtx_Id, Signal Name, Samp Intvl, "
              << "Block Size" << endl;

   for( sig_num = 0; sig_num < num_nodes; sig_num++) {
      sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;
      *DebugFile << sig_num << ")  "
                 << sig_id << "  "
                 << sig_id->GetName() << "  "
                 << (Sdg_Vert_Descr->at(sig_num))->samp_intvl << "  "
                 << (Sdg_Vert_Descr->at(sig_num))->block_size << endl;
   }

   *DebugFile << "\nEdges:" << endl;
   *DebugFile << "index, SDG edge model, Model Name, Resamp Rate, "
              << "Delay" << endl;
   for( edge_num = 0; edge_num < num_edges; edge_num ++) {
      model_id = (Sdg_Edge_Descr->at(edge_num))->model_id;
      *DebugFile << edge_num << ")  "
                 << model_id << "  "
                 << model_id->GetModelName() << ":" 
                 << model_id->GetInstanceName() << "  "
                 << (Sdg_Edge_Descr->at(edge_num))->resamp_rate << "  "
                 << (Sdg_Edge_Descr->at(edge_num))->delta_delay << endl;
   }

   *DebugFile << "\nAdjacency Matrix:" << endl;
   for( sig_num = 0; sig_num < num_nodes; sig_num++) {
      for( sig_num2 = 0; sig_num2 < num_nodes; sig_num2++) {
         edge_num = Sig_Dep_Graph->GetEdgeNum(sig_num, sig_num2);

         if( edge_num >= 0 ) {
        *DebugFile << "Edge " << edge_num
                   << " connects from vertex " << sig_num
                   << " to vertex " << sig_num2 << endl;
         }
      }
   }
}
//===================================================================

void SystemGraph::ResolveSignalParms(void)
{
  int sig_num, unsorted_sig_num, base_sig_num, edge_num;
  int num_base_sigs_used = 0;
  bool base_sig_not_found;
  double samp_rate;
  int block_size;
  GenericSignal *sig_id;
  bool is_forward;

  int num_nodes = Sig_Dep_Graph->GetNumVerts();
  bool *used_as_base = new bool[num_nodes];

  for( sig_num = 0; sig_num < num_nodes; sig_num++)
    {
    used_as_base[sig_num] = false;
    }
  this->TopoSortSDG();

  while(num_base_sigs_used < Num_Regular_Sigs)
    {
    //---------------------------------------------------------------
    // find base signal

    base_sig_not_found = true;
    for(unsorted_sig_num = 0; unsorted_sig_num < num_nodes; unsorted_sig_num++)
      {
      sig_num = Sorted_Sig_Nodes[unsorted_sig_num];

      sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;
      samp_rate = (Sdg_Vert_Descr->at(sig_num))->samp_intvl;
      if(samp_rate <= 0.0)
        {
        #ifdef _DEBUG
        *DebugFile << sig_id->GetName()
                  << " rejected because samp rate <= 0 ( "
                  << samp_rate << " )" << endl;
        #endif
        continue;
        }

      block_size = (Sdg_Vert_Descr->at(sig_num))->block_size;
      if(block_size <= 0.0)
        {
        #ifdef _DEBUG
        *DebugFile << sig_id->GetName()
                  << " rejected because block size <= 0 ( "
                  << block_size << " )" << endl;
        #endif
        continue;
        }

      if(used_as_base[sig_num])
        {
        #ifdef _DEBUG
        *DebugFile << sig_id->GetName()
                  << " rejected because already used as base"
                  << endl;
        #endif
        continue;
        }
      //
      // if execution gets to here, we have found a useable base signal
      //
      base_sig_num = sig_num;
      used_as_base[base_sig_num] = true;
      num_base_sigs_used++;
      base_sig_not_found = false;
      break;
      } // end of loop over unsorted_sig_num

    if(base_sig_not_found)
      {
      (*DebugFile) << "Unable to find suitable base signal" << endl;
      #ifdef _DEBUG
        *DebugFile << "The following signals have not"
                  << " been used as base signals:" << endl;
        for(unsorted_sig_num = 0; unsorted_sig_num < num_nodes; unsorted_sig_num++)
          {
          sig_num = Sorted_Sig_Nodes[unsorted_sig_num];
          if(used_as_base[sig_num]) continue;
            sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;
            *DebugFile << "   " << sig_id->GetName() << endl;
          }
      #endif
      exit(0);
      }

    #ifdef _DEBUG
      sig_id = (Sdg_Vert_Descr->at(base_sig_num))->signal_id;
      *DebugFile << "\nbase signal is " << sig_id->GetName() << endl;
    #endif

    //---------------------------------------------------------
    //  Work down the column corresponding to the base signal to
    //  find inbound edges.  For each edge found, attempt to 
    //  propagate parameters back to the edge's source signal

    for( sig_num = 0; sig_num < num_nodes; sig_num++)
      {
      edge_num = Sig_Dep_Graph->GetEdgeNum( sig_num, base_sig_num );
      if(edge_num < 0 ) continue;
        //
        //else
        is_forward = false;
        #ifdef _DEBUG
          sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;
          *DebugFile << "looking backward to " << sig_id->GetName() << endl;
        #endif
        Propagate( base_sig_num, edge_num, sig_num, is_forward);
      }

    //-------------------------------------------------------------
    //  Work across the row corresponding to the base signal to
    //  find outbound edges.  For each edge found, attempt to
    //  propagate parameters forward to the edge's destination signal.

    for( sig_num = 0; sig_num < num_nodes; sig_num++)
      {
      edge_num = Sig_Dep_Graph->GetEdgeNum( base_sig_num, sig_num );
      if(edge_num < 0 ) continue;
        //
        //else
        is_forward = true;
        #ifdef _DEBUG
          sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;
          *DebugFile << "looking forward to " << sig_id->GetName() << endl;
        #endif
        Propagate( base_sig_num, edge_num, sig_num, is_forward);
      }
    } // end of while(num_base_sigs_used < Num_Regular_Sigs)
  return;
}
//============================================================
void SystemGraph::TopoSortSDG(void)
{
  int num_nodes = Sig_Dep_Graph->GetNumVerts();
  Sorted_Sig_Nodes = new int[num_nodes];
  for(int i=0; i<num_nodes; i++)
    {
    Sorted_Sig_Nodes[i] = i;
    }
  return;
}
//=======================================================================
void SystemGraph::Propagate( int base_sig_num,
                                   int edge_num,
                                   int sig_num,
                                   bool is_forward )
{
  double t_base, t_sig;
  double r_edge;
  int n_base, n_sig;
  int r_def, t_def, n_def;
  int config_key;
  bool is_const_intvl;

  t_base = (Sdg_Vert_Descr->at(base_sig_num))->samp_intvl;
  n_base = (Sdg_Vert_Descr->at(base_sig_num))->block_size;
  r_edge = (Sdg_Edge_Descr->at(edge_num))->resamp_rate;
  is_const_intvl = (Sdg_Edge_Descr->at(edge_num))->is_const_intvl;
  t_sig = (Sdg_Vert_Descr->at(sig_num))->samp_intvl;
  n_sig = (Sdg_Vert_Descr->at(sig_num))->block_size;

  r_def = 0;
  t_def = 0;
  n_def = 0;
  if( r_edge > 0.0 ) r_def = 1;
  if( t_sig > 0.0) t_def = 2;
  if( n_sig >0 ) n_def = 4;

  config_key = r_def + t_def + n_def;
  if(is_forward) config_key +=8;
  if(is_const_intvl) config_key +=16;

  switch (config_key)
    {
    case 0: //backward, nothing defined
      // no action possible
      #ifdef _DEBUG
        *DebugFile << "  case 0 - no action possible" << endl;
      #endif
      return;
    case 1:  //backward, resampling rate is defined
      // compute sampling rate : Tn = R * Tb
      // compute block size : Nn = Nb / R

      t_sig = r_edge * t_base;
      n_sig = int(n_base / r_edge);
      #ifdef _DEBUG
        *DebugFile << " case 1:\n"
                  << "   new samp intvl = " << t_sig
                  << "   new block size = " << n_sig
                  << endl;
      #endif

      break;
    case 2:   //backward, sampling interval defined
      // compute resampling rate : R = Tn / Tb
      // compute block size : Nn = Nb / R

      r_edge = t_sig/t_base;
      n_sig = int(n_base/r_edge);
      #ifdef _DEBUG
        *DebugFile << " case 2:\n"
                  << "   new block size = " << n_sig
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 3:   //backward, resampling rate and 
              //sampling interval are defined
      // compute block size : Nn = Nb / R
      // check sampling interval : (Tn == R * Tb)?

      n_sig = int(n_base / r_edge);
      if( t_sig != (r_edge * t_base))
        { // error
        #ifdef _DEBUG
          *DebugFile << " error in case 3" << endl;
        #endif
        ErrorStream << "\n\n********* FATAL ERROR ************" << endl;
        ErrorStream << "resampling rate in instance '"
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetInstanceName() 
                      << "' of model '" 
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetModelName() 
                      << "'\n is inconsistent input/output sampling intervals" << endl;
        exit(0);
        }
      #ifdef _DEBUG
        *DebugFile << " case 3:\n"
                  << "   new block size = " << n_sig
                  << endl;
      #endif
      break;
    case 4:   //backward, block size is defined
      // compute resampling rate : R = Nb / Nn
      // compute sampling interval : Tn = R * Tb

      r_edge = float(n_base) / n_sig;
      t_sig = r_edge * t_base;
      #ifdef _DEBUG
        *DebugFile << " case 4:\n"
                  << "   new samp interval = " << t_sig
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 5:   //backward, resampling rate and block size defined
      // compute sampling interval : Tn = R * Tb
      // check block size : (Nb == R * Nn)?

      t_sig = r_edge * t_base;
      if( n_base != (r_edge * n_sig))
        { // error
        #ifdef _DEBUG
          *DebugFile << " error in case 5" << endl;
        #endif
        ErrorStream << "\n\n********* FATAL ERROR ************" << endl;
        ErrorStream << "resampling rate in instance '"
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetInstanceName() 
                      << "' of model '" 
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetModelName() 
                      << "'\n is inconsistent with input/output block sizes" << endl;
        exit(0);
        }
      #ifdef _DEBUG
        *DebugFile << " case 5:\n"
                  << "   new samp interval = " << t_sig
                  << endl;
      #endif
      break;
    case 6:   //backward, sampling interval and block size defined
      // compute resampling rate : R = Tn / Tb
      // check block size : (Nb == R * Nn)?

      r_edge = t_sig / t_base;
      if( n_base != (r_edge * n_sig))
        { //error
        #ifdef _DEBUG
          *DebugFile << " error in case 6" << endl;
        #endif
        ErrorStream << "\n\n********* FATAL ERROR ************" << endl;
        ErrorStream << "resampling rate in instance '"
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetInstanceName() 
                      << "' of model '" 
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetModelName() 
                      << "'\n is inconsistent with input/output block sizes" << endl;
        exit(0);
        }
      #ifdef _DEBUG
        *DebugFile << " case 6:\n"
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 7:   //backward, all parameters are defined
      // check : (Tn == R * Tb)?
      // check : (Nb == R * Nn)?

      if( t_sig != (r_edge * t_base))
        { // error
        #ifdef _DEBUG
          *DebugFile << " error in case 7a" << endl;
        #endif
        ErrorStream << "\n\n********* FATAL ERROR ************" << endl;
        ErrorStream << "resampling rate in instance '"
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetInstanceName() 
                      << "' of model '" 
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetModelName() 
                      << "'\n is inconsistent with input/output sampling intervals" << endl;
        exit(0);
        }
      if( n_base != ceil(r_edge * n_sig))
        { // error
        #ifdef _DEBUG
          *DebugFile << " error in case 7b" << endl;
        #endif
        ErrorStream << "\n\n********* FATAL ERROR ************" << endl;
        ErrorStream << "resampling rate in instance '"
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetInstanceName() 
                      << "' of model '" 
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetModelName() 
                      << "'\n is inconsistent with input/output block sizes" << endl;
        ErrorStream << "n_base = " << n_base << endl;
        ErrorStream << "r_edge = " << r_edge << endl;
        ErrorStream << "n_sig = " << n_sig << endl;
        exit(0);
        }
      #ifdef _DEBUG
        *DebugFile << " case 7 - all parameters defined" << endl;
      #endif
      break;
    case 8:   //forward, nothing defined
      // no action possible
      #ifdef _DEBUG
        *DebugFile << " case 8 - no action possible" << endl;
      #endif
      return;
    case 9:   //forward, resampling rate is defined
      // compute sampling interval : Tn = Tb / R
      // compute block size : Nn = R * Nb

      t_sig = t_base / r_edge;
      n_sig = int(ceil(r_edge * n_base));
      #ifdef _DEBUG
        *DebugFile << " case 9:\n"
                  << "   new samp interval = " << t_sig
                  << "   new block size = " << n_sig
                  << endl;
      #endif
      break;
    case 10:    //forward, sampling interval defined
      // compute resampling rate : R = Tb / Tn
      // compute block size : Nn = R * Nb

      r_edge = t_base / t_sig;
      n_sig = int(r_edge * n_base);
      #ifdef _DEBUG
        *DebugFile << " case 10:\n"
                  << "   new block size = " << n_sig
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 11:    //forward, resampling rate and
                // sampling interval defined
      // compute block size : Nn = R * Nb
      // check sampling interval : (Tb == R * Tn)?

      n_sig = int(r_edge * n_base);
      if(t_base != r_edge * t_sig)
        { // error
        #ifdef _DEBUG
          *DebugFile << " error in case 11" << endl;
        #endif
        ErrorStream << "\n\n********* FATAL ERROR ************" << endl;
        ErrorStream << "resampling rate in instance '"
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetInstanceName() 
                      << "' of model '" 
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetModelName() 
                      << "'\n is inconsistent with input/output sampling intervals" << endl;
        exit(0);
        }
      #ifdef _DEBUG
        *DebugFile << " case 11:\n"
                  << "   new samp interval = " << t_sig
                  << "   new block size = " << n_sig
                  << endl;
      #endif
      break;
    case 12:    //forward, block size is defined
      // compute resampling rate : R = Nn / Nb
      // compute sampling interval : Tn = Tb / R

      r_edge = n_sig / n_base;
      t_sig = t_base / r_edge;
      #ifdef _DEBUG
        *DebugFile << " case 12:\n"
                  << "   new samp interval = " << t_sig
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 13:    //forward, resampling rate and
                // block size are defined
      // compute sampling interval : Tn = Tb / R
      // check block size : (Nn == R * Nb)?

      t_sig = t_base / r_edge;
      if(n_sig != (r_edge * n_base))
        { // error
        #ifdef _DEBUG
          *DebugFile << " error in case 13" << endl;
        #endif
        ErrorStream << "\n\n********* FATAL ERROR ************" << endl;
        ErrorStream << "resampling rate in instance '"
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetInstanceName() 
                      << "' of model '" 
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetModelName() 
                      << "'\n is inconsistent with input/output block sizes" << endl;
        exit(0);
        }
      #ifdef _DEBUG
        *DebugFile << " case 13:\n"
                  << "   new samp interval = " << t_sig
                  << endl;
      #endif
      break;
    case 14:    //forward, sampling interval and
                // block size are defined
      // compute resampling rate : R = Tb / Tn
      // check block size : (Nn == R * Nb)?

      r_edge = t_base / t_sig;
      if( n_sig != (r_edge * n_base))
        { // error
        #ifdef _DEBUG
          *DebugFile << " error in case 14" << endl;
          *DebugFile << "   ( n_sig != (r_edge * n_base))" << endl;
          *DebugFile << "    n_sig = " << n_sig << endl;
          *DebugFile << "    r_edge = " << r_edge << endl;
          *DebugFile << "    n_base = " << n_base << endl;
        #endif
        ErrorStream << "\n\n********* FATAL ERROR ************" << endl;
        ErrorStream << "resampling rate in instance '"
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetInstanceName() 
                      << "' of model '" 
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetModelName() 
                      << "'\n is inconsistent with input/output block sizes" << endl;
        exit(0);
        }
      #ifdef _DEBUG
        *DebugFile << " case 14:\n"
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 15:    //forward, all parameters are defined
      // check : (Tb == R * Tn)?
      // check : (Nn == R * Nb)?

      if( t_base != (r_edge * t_sig))
        { // error
        #ifdef _DEBUG
          *DebugFile << " error in case 15a" << endl;
        #endif
        ErrorStream << "\n\n********* FATAL ERROR ************" << endl;
        ErrorStream << "resampling rate in instance '"
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetInstanceName() 
                      << "' of model '" 
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetModelName() 
                      << "'\n is inconsistent with input/output sampling intervals" << endl;
        exit(0);
        }
      if( n_sig != (r_edge * n_base))
        { // error
        #ifdef _DEBUG
          *DebugFile << " error in case 15b" << endl;
        #endif
        ErrorStream << "\n\n********* FATAL ERROR ************" << endl;
        ErrorStream << "resampling rate in instance '"
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetInstanceName() 
                      << "' of model '" 
                      << ((Sdg_Edge_Descr->at(edge_num))->model_id)->GetModelName() 
                      << "'\n is inconsistent with input/output block sizes" << endl;
        ErrorStream << "n_base = " << n_base << endl;
        ErrorStream << "r_edge = " << r_edge << endl;
        ErrorStream << "n_sig = " << n_sig << endl;
        exit(0);
        }
      #ifdef _DEBUG
        *DebugFile << " case 15 - all parameters defined" << endl;
      #endif
      break;
    case 16: //backward, constant interval,  nothing defined
      // no action possible
      #ifdef _DEBUG
        *DebugFile << "  case 16 - no action possible" << endl;
      #endif
      return;
    case 17:  //backward, constant interval, resampling rate is defined
      // constant sampling rate : Tn = Tb
      // compute block size : Nn = ceil(Nb / R)

      t_sig = t_base;
      n_sig = int(ceil(n_base / r_edge));
      #ifdef _DEBUG
        *DebugFile << " case 17:\n"
                  << "   new block size = " << n_sig
                  << endl;
      #endif

      break;
    case 18:   //backward, constant interval, sampling interval defined
      // compute resampling rate : R = Tn / Tb
      //      this is not really constant interval if Tn != Tb
      // compute block size : Nn = ceil(Nb / R)

      r_edge = t_sig/t_base;
      n_sig = int(ceil(n_base/r_edge));
      #ifdef _DEBUG
        *DebugFile << " case 18:\n"
                  << "   new block size = " << n_sig
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 19:   //backward, constant interval, resampling rate and 
              //sampling interval are defined
      // compute block size : Nn = ceil(Nb / R)

      n_sig = int(ceil(n_base / r_edge));
      #ifdef _DEBUG
        *DebugFile << " case 19:\n"
                  << "   new block size = " << n_sig
                  << endl;
      #endif
      break;
    case 20:   //backward, constant interval, block size is defined
      // compute resampling rate : R = Nb / Nn
      // constant sampling interval : Tn = Tb

      r_edge = float(n_base) / n_sig;
      t_sig = t_base;
      #ifdef _DEBUG
        *DebugFile << " case 20:\n"
                  << "   new samp interval = " << t_sig
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 21:   //backward, constant interval, resampling rate and block size defined
      // constant sampling interval : Tn = Tb

      t_sig = t_base;
      #ifdef _DEBUG
        *DebugFile << " case 21:\n"
                  << "   new samp interval = " << t_sig
                  << endl;
      #endif
      break;
    case 22:   //backward, constant interval, sampling interval and block size defined
      // compute resampling rate : R = Nb / Nn

      r_edge = n_base / n_sig;
      #ifdef _DEBUG
        *DebugFile << " case 22:\n"
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 23:   //backward, constant interval, all parameters are defined
      #ifdef _DEBUG
        *DebugFile << " case 23 - all parameters defined" << endl;
      #endif
      break;
    case 24:   //forward, constant interval, nothing defined
      // no action possible
      #ifdef _DEBUG
        *DebugFile << " case 24 - no action possible" << endl;
      #endif
      return;
    case 25:   //forward, constant interval, resampling rate is defined
      // constant sampling interval : Tn = Tb
      // compute block size : Nn = ceil(R * Nb)

      t_sig = t_base;
      n_sig = int(ceil(r_edge * n_base));
      #ifdef _DEBUG
        *DebugFile << " case 25:\n"
                  << "   new samp interval = " << t_sig
                  << "   new block size = " << n_sig
                  << endl;
      #endif
      break;
    case 26:    //forward, constant interval, sampling interval defined
      // compute resampling rate : R = Tb / Tn
      // compute block size : Nn = ceil(R * Nb)

      r_edge = t_base / t_sig;
      n_sig = int(ceil(r_edge * n_base));
      #ifdef _DEBUG
        *DebugFile << " case 26:\n"
                  << "   new block size = " << n_sig
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 27:    //forward, constant interval, resampling rate and
                // sampling interval defined
      // compute block size : Nn = ceil(R * Nb)

      n_sig = int(ceil(r_edge * n_base));
      #ifdef _DEBUG
        *DebugFile << " case 27:\n"
                  << "   new block size = " << n_sig
                  << endl;
      #endif
      break;
    case 28:    //forward, constant interval, block size is defined
      // compute resampling rate : R = Nn / Nb
      // constant sampling interval : Tn = Tb

      r_edge = n_sig / n_base;
      t_sig = t_base;
      #ifdef _DEBUG
        *DebugFile << " case 28:\n"
                  << "   new samp interval = " << t_sig
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 29:    //forward, constant interval, resampling rate and
                // block size are defined
      // constant sampling interval : Tn = Tb
      // check block size : (Nn == R * Nb)?

      t_sig = t_base;
      #ifdef _DEBUG
        *DebugFile << " case 29:\n"
                  << "   new samp interval = " << t_sig
                  << endl;
      #endif
      break;
    case 30:    //forward, constant interval, sampling interval and
                // block size are defined
      // compute resampling rate : R = Nn / Nb

      r_edge = n_sig / n_base;
      #ifdef _DEBUG
        *DebugFile << " case 30:\n"
                  << "   new resamp rate = " << r_edge
                  << endl;
      #endif
      break;
    case 31:    //forward, constant interval, all parameters are defined

      #ifdef _DEBUG
        *DebugFile << " case 31 - all parameters defined" << endl;
      #endif
      break;
    default:
      *DebugFile << "Fatal Error in Propagate()" << endl;
      exit(0);
      
    } // end of switch on config_key
      // check for integer block size

//  if( (int(r_edge * n_sig)) != n_base )
//    { // errror
//    *DebugFile << "base block size = " << n_base << endl;
//    *DebugFile << "resampling rate = " << r_edge << endl;
//    *DebugFile << "calculated block size = " << (float(n_base)/r_edge) 
//              << endl;
//    }
  (Sdg_Vert_Descr->at(sig_num))->samp_intvl = t_sig;
  (Sdg_Vert_Descr->at(sig_num))->block_size = n_sig;
  (Sdg_Edge_Descr->at(edge_num))->resamp_rate = r_edge;
  return;
}
//====================================================================
//  Set the propagated values of signal parameters within each
//  individual signal object
void SystemGraph::DistributeSignalParms(void)
{
  int sig_num, block_size;
  GenericSignal *sig_id;
  double samp_intvl;
  int num_nodes = Sig_Dep_Graph->GetNumVerts();

  for( sig_num = 0; sig_num < num_nodes; sig_num++)
    {
    if( ((Sdg_Vert_Descr->at(sig_num))->kind_of_signal) != SK_REGULAR_SIGNAL) continue;
      //
      // else
      sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;

      block_size = (Sdg_Vert_Descr->at(sig_num))->block_size;
      sig_id->SetBlockSize( block_size );

      //buffer_size = (Sdg_Vert_Descr->at(sig_num))->buffer_size;
      //sig_id->SetBufferSize( buffer_size );

      samp_intvl = (Sdg_Vert_Descr->at(sig_num))->samp_intvl;
      sig_id->SetSampIntvl( samp_intvl );
    }
  return;
}
//====================================================================
void SystemGraph::AllocateStorageBuffers(void)
{
  int sig_num;
  GenericSignal *sig_id;
  int num_nodes = Sig_Dep_Graph->GetNumVerts();

  for(sig_num = 0; sig_num < num_nodes; sig_num++)
    {
    //------------------------------------------------
    // don't consider dummy signals

    if( ((Sdg_Vert_Descr->at(sig_num))->kind_of_signal) != SK_REGULAR_SIGNAL) continue;
      //
      // else
      sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;
      sig_id->AllocateSignalBuffer();
    }
}
//============================================================
void SystemGraph::InitializeReadPtrs(void)
{
  GenericSignal* sig_id;
  int num_sigs = Sig_Dep_Graph->GetNumVerts();
  for(int sig_num = 0; sig_num < num_sigs; sig_num++)
    {
    if( ((Sdg_Vert_Descr->at(sig_num))->kind_of_signal) != SK_REGULAR_SIGNAL ) continue;
        //
        // else
        sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;
        sig_id->InitializeReadPtrs();
    }
}
//===================================================================
void SystemGraph::InitializeModels(void)
{
  PracSimModel *model_id;
  *DebugFile << "In SystemGraph::InitializeModels()" << endl;
  EnclaveNumber = 0;
  int num_edges = Sig_Dep_Graph->GetNumEdges();
  for( int edge_num = 0; edge_num < num_edges; edge_num ++)
    {
    model_id = (Sdg_Edge_Descr->at(edge_num))->model_id;
    // note: this might pick up models more than once!

    model_id->Initialize();
    *DebugFile << "Initialized model " << model_id->GetModelName() << ":"
               << model_id->GetInstanceName() << endl;
    }
}
//==============================================================================
void SystemGraph::DeleteModels(void)
{
  PracSimModel *model_id;
  *DebugFile << "In SystemGraph::InitializeModels()" << endl;
  int num_edges = Sig_Dep_Graph->GetNumEdges();
  for(int model_num = 0; model_num < Num_Sys_Lev_Models; model_num++)
    {
    model_id = Syst_Lev_Models->at(model_num);

    #ifdef _DEBUG
    *DebugFile << "Deleting model " << model_id->GetModelName() << ":"
               << model_id->GetInstanceName() << endl;
    #endif
    delete model_id;
    }
}
//==============================================================================
void SystemGraph::RunSimulation(void)
{
  int model_num;
  int model_exec_status;

  EnclaveNumber = 0;
  EnclaveOffset[0] = 0;
  for(model_num = 0; model_num < Num_Sys_Lev_Models; model_num++)
    {
    ActiveModel = Syst_Lev_Models->at(model_num);
    #ifdef _DEBUG
      *DebugFile << "ASG launching Execute for " << ActiveModel->GetModelName() 
                  << ":" << ActiveModel->GetInstanceName() << endl;
    #endif
    model_exec_status = ActiveModel->Execute();
    if( model_exec_status == _MES_AOK ) continue;
      // else take action appropriate for returned status code
      switch (model_exec_status)
        {
        case _MES_RESTART:
          *DebugFile << "Restarting at top of Enclave 0" << endl;
          goto signal_update;
        default: //error
          cout << "unrecognized status returned by model execute method" << endl;
          exit(1);
        } // end of switch on model_exec_status
    }
  SigPlot.CollectData();
  //----------------------------------------------------------------------
  // Update signals
signal_update:
  int num_nodes = Sig_Dep_Graph->GetNumVerts();
  GenericSignal *sig_id;

  for( int sig_num = 0; sig_num < num_nodes; sig_num++)
    {
    if( ((Sdg_Vert_Descr->at(sig_num))->kind_of_signal) != SK_REGULAR_SIGNAL) continue;
      //
      // else
      sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;
      sig_id->PassUpdate();
    }
}
//=========================================================================
void SystemGraph::RegisterModel(PracSimModel* model)
{
  if(model->GetNestDepth() == 1)
    {
    // add model to system level list
    Syst_Lev_Models->push_back(model);
    Num_Sys_Lev_Models++;
    }
}
//================================================================
void SystemGraph::AllocatePlotPointers(void)
{
  GenericSignal* sig_id;
  int num_sigs = Sig_Dep_Graph->GetNumVerts();
  for(int sig_num = 0; sig_num < num_sigs; sig_num++)
    {
    sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;
    sig_id->SetupPlotSignal();
    }
}
//==========================================================
GenericSignal* SystemGraph::GetSignalId( char* sig_name)
{
  GenericSignal *sig_id;
  int num_sigs = Sig_Dep_Graph->GetNumVerts();
  for(int sig_num = 0; sig_num < num_sigs; sig_num++)
    {
    sig_id = (Sdg_Vert_Descr->at(sig_num))->signal_id;
    if( strcmp( sig_name, sig_id->GetName())) continue;
      return(sig_id);      
    }
  return(NULL);
}