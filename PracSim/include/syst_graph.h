//
//  File = sysgraph.h
//

#ifndef _SYSGRAPH_H_
#define _SYSGRAPH_H_

#include "model_graph.h"
#include "digraph.h"

typedef struct{
  GenericSignal*     signal_id;
  int                block_size;
  double             samp_intvl;
  SignalKinds_type   kind_of_signal;
  } sdg_sig_desc_type;

typedef struct{
  PracSimModel*     model_id;
  double            resamp_rate;
  double            delta_delay;
  bool              is_const_intvl;
  } sdg_edge_desc_type;

class SystemGraph
{
public:
  SystemGraph(void);

  ~SystemGraph(void);

  void MergeCurrModelGraph( ModelGraph* cmsg );
  void DumpSDGraph(void);
  void ResolveSignalParms(void);
  void TopoSortSDG(void);
  void Propagate( int base_sig_num,
                  int edge_num,
                  int sig_num,
                  bool is_forward);
  void DistributeSignalParms(void);
  void AllocateStorageBuffers(void);
  void InitializeReadPtrs(void);
  void DeleteModels(void);
  void InitializeModels(void);
  void RunSimulation(void);
  void RegisterModel(PracSimModel* model);
  void AllocatePlotPointers(void);
  GenericSignal* GetSignalId( char* sig_name);

private:
  std::vector<sdg_sig_desc_type*> *Sdg_Vert_Descr;
  std::vector<sdg_edge_desc_type*> *Sdg_Edge_Descr;
  std::vector<double> *Samp_Intvl;
  std::vector<int> *Block_Size;
  DirectedGraph *Sig_Dep_Graph;
  int Num_Regular_Sigs;
  int *Sorted_Sig_Nodes;
  std::vector<PracSimModel*> *Syst_Lev_Models;
  int Num_Sys_Lev_Models;
  
};

#endif //_SYSGRAPH_H_
