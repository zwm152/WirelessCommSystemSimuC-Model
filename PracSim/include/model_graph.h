//
//  File = modgraph.h
//
#ifndef _MODGRAPH_H_
#define _MODGRAPH_H_
//#include "gensig.h"
#include "vector"
#include "psmodel.h"
#include "digraph.h"

enum SignalKinds_type {
    SK_DUMMY_SIGNAL,
    SK_REGULAR_SIGNAL,
    SK_DUMMY_SOURCE_SIGNAL,
    SK_DUMMY_DEST_SIGNAL,
    SK_GENERAL_CONTROL,
    SK_DUMMY_SOURCE_CONTROL,
    SK_DUMMY_DEST_CONTROL};

//typedef SignalKinds_enum SignalKinds_type;

#define UNDEFINED_RATE -1.0

class ModelGraph
{
public:
  ModelGraph(PracSimModel *owning_model);
  ~ModelGraph(void);
  void InsertSignal(GenericSignal *new_sig_id,
                    PracSimModel *model_ptr,
                    bool sig_is_input);
  GenericSignal* GetSignalId(int vtx_num);
  obj_id_type GetVertexId(int vtx_num);
  PracSimModel* GetModelId(int edge_num);
  obj_id_type GetEdgeId(int vtx_num);
  int GetEdgeNum(int fm_vtx_num, int to_vtx_num);
  void EnableMultirate(void);
  void EnableConstantInterval(void);
  SignalKinds_type GetVertexKind(int vtx_num);
  void ChangeRate(  GenericSignal* in_sig,
                    GenericSignal* out_sig,
                    double resamp_rate,
                    PracSimModel* model_ptr);
  void Closeout( PracSimModel* model_ptr);
  int GetNumVerts(void);
  double GetSampRate(int vtx_num);
  double GetResampRate(int edge_num);
  bool GetIsConstIntvl(int edge_num);
  double GetDelay(int edge_num);
  int GetBlockSize(int vtx_num);
   void SetSampIntvl( GenericSignal* sig_id,
                     double samp_intvl);
   double GetSampIntvl(int vtx_num);
  void SetBlockSize( GenericSignal* sig_id,
                     int block_size);
  bool ConnIsFeedback(int vtx_num);
  void DumpSigDepGraph(void);
  PracSimModel* GetOwningModel(void);

private:
  int Max_Signals;
  PracSimModel *Owning_Model;
  bool Model_Is_Multirate;
  bool Model_Is_Constant_Interval;
  std::vector<bool> *Vertex_Is_Input;
  //std::vector<bool> *Vertex_Is_Internal;
  std::vector<SignalKinds_type> *Vertex_Kind;
  std::vector<bool> *Node_Is_Feedback;
  std::vector<int> *Block_Size;
  std::vector<double> *Samp_Intvl;
  std::vector<double> *Delta_Delay;
  std::vector<double> *Resamp_Rate;
  std::vector<bool> *Const_Intvl;
  DirectedGraph* Sig_Dep_Graph;

};
#endif // _MODGRAPH_H_