//
//  File = exec.cpp
//

#include <stdlib.h>
#include <fstream>
#include "exec.h"
#include "syst_graph.h"
#include "model_graph.h"
#include "sigplot.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

extern SystemGraph CommSystemGraph;
extern SignalPlotter SigPlot;
extern int PassNumber;
extern PracSimModel *PrevModelConstr;

//============================================
// constructor

Executive::Executive()
{
  return;
}
//============================================
// destructor

Executive::~Executive()
{
}
//============================================

void Executive::MultirateSetup(void)
{
    PrevModelConstr->CloseoutModelGraph(2);
    PrevModelConstr = NULL;
  //SigMgr.DumpInputs();
  //SigMgr.DumpInternals();
  //SigMgr.DumpOutputs();
///////////////////////////  SigMgr.LinkAliases();
  //SigMgr.DumpInputs();

  //ControlMgr.DumpInputs();
  //ControlMgr.DumpOutputs();
  //ControlMgr.LinkAliases();
  //ControlMgr.DumpInputs();
  //ControlMgr.DumpOutputs();

 //////////////////////////////// CommSystemGraph.BuildGraph();

  //CommSystemGraph.BuildModelDepGraph();

  SigPlot.Initialize();
//////////////////////////////////  ReInit.Initialize();

  //--------------------------------------------
  // Dump contents of the SDG to the debug file

  CommSystemGraph.DumpSDGraph();

  //-------------------------------------------
  //  Dump contents of the MDG to the debug file

  //CommSystemGraph.DumpMDGraph();

  //CommSystemGraph.CollapseModelDepGraph();

  //CommSystemGraph.DumpMDGraph();

  //CommSystemGraph.TopoSortMDG();

  //-----------------------------------------------
  // Propagate signal parameters throughout the SDG

  CommSystemGraph.ResolveSignalParms();

  //----------------------------------------------
  //  Dump contents of the SDG to the debug file

  CommSystemGraph.DumpSDGraph();

  //----------------------------------------------
  //  Propagate feedforward delays throughout the SDG

  //CommSystemGraph.ResolveDelays();

  //------------------------------------------------
  //  Issue Multirate configuration report

///////////////////////////////////////////  CommSystemGraph.DumpMultirateInfo();

  //------------------------------------------------
  //  Distribute the signal parameters out to
  //  the individual node objects.

  CommSystemGraph.DistributeSignalParms();

  //-----------------------------------------------
  // Allocate node buffer arrays for each active node

  CommSystemGraph.AllocateStorageBuffers();

  //-------------------------------------------------
  //  Initialize node buffer read pointers for 
  //  each input node connection

//  SigMgr.AllocateReadPointers();
  CommSystemGraph.InitializeReadPtrs();

  //------------------------------------------------
  //  Initialize signal plotting pointers

//  SigMgr.AllocatePlotPointers();
  CommSystemGraph.AllocatePlotPointers();

  //-----------------------------------------------
  //  Initialize each model that has been registered in
  //  the active system graph by the model's constructor
  //

///  CommSystemGraph.SetupSimulation();
  CommSystemGraph.InitializeModels();
  // This just runs the Initialize method for each model

  //---------------------------------------------
  // run second initilaiztions

/////////////////////////////////////////////////////  CommSystemGraph.SecondInit();

  return;
}