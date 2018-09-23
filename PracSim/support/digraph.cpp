//
//  File = digraph.cpp
//

#include <stdlib.h>
#include <fstream>
#include "digraph.h"
#ifdef _DEBUG
extern std::ofstream *DebugFile;
#endif

//===============================================

DirectedGraph::DirectedGraph( void )
{
  Vertex_List = new std::vector<obj_id_type>;
  Edge_List = new std::vector<obj_id_type>;
  Row_List = new std::deque<std::vector<int>* >;
  Num_Verts = 0;
  Num_Edges = 0;
}
//===============================================
DirectedGraph::~DirectedGraph( void )
{
  delete Vertex_List;
  delete Edge_List;
};
//===============================================
int DirectedGraph::GetNumVerts( void )
{
  return(Num_Verts);
};
//===============================================
int DirectedGraph::GetNumEdges( void )
{
  return(Num_Edges);
};
//===============================================
int DirectedGraph::AddVertex(obj_id_type vertex_id)
{
  int i;
  
  Vertex_List->push_back(vertex_id);
  //------------------------------------
  //  Add row to adjacency matrix

  std::vector<int>* new_row = new std::vector<int>;
  for(i=0; i<Num_Verts; i++)
    {
    new_row->push_back(-1);
    }
  Row_List->push_back(new_row);

  //------------------------------------
  //  Add column to adjacency matrix

  for(i=0; i<=Num_Verts; i++)
    {
    (Row_List->at(i))->push_back(-1);
    }
  Num_Verts++;

  return(Num_Verts-1);
}
//================================================
int DirectedGraph::AddEdge( obj_id_type edge_id,
                            int fm_vtx_num,
                            int to_vtx_num)
{
  Edge_List->push_back(edge_id);

  // insert new_edge_num (==Num_Edges) into
  // adjacency matrix at row fm_vtx_num and column to_vtx_num
  //
  // pointer to the desired row vector: Row_List->at(fm_vtx_num)

  (Row_List->at(fm_vtx_num))->at(to_vtx_num) = Num_Edges;

  Num_Edges++;
  return(Num_Edges-1);
};
//================================================
int DirectedGraph::AddEdge( obj_id_type edge_id,
                            obj_id_type fm_vtx_id,
                            obj_id_type to_vtx_id)
{
  int i, fm_vtx_num, to_vtx_num;
  
  Edge_List->push_back(edge_id);

  // search Vertex_List to find vertex number corresponding
  // to fm_vtx_id

  fm_vtx_num = -1;
  for(i=0; i<Num_Verts; i++)
    {
    if(Vertex_List->at(i) == fm_vtx_id) fm_vtx_num = i;
    }
  if(fm_vtx_num < 0)
    {
    // error: source vertex not found in graph
    return(-1);
    }

  // search Vertex_List to find vertex number corresponding
  // to to_vtx_id

  to_vtx_num = -1;
  for(i=0; i<Num_Verts; i++)
    {
    if(Vertex_List->at(i) == to_vtx_id) to_vtx_num = i;
    }
  if(to_vtx_num < 0)
    {
    // error: destination vertex not found in graph
    return(-1);
    }

  // insert new_edge_num (==Num_Edges) into
  // adjacency matrix at row fm_vtx_num and column to_vtx_num

  (Row_List->at(fm_vtx_num))->at(to_vtx_num) = Num_Edges;

  Num_Edges++;
  return(Num_Edges-1);
};
//================================================
int DirectedGraph::GetEdgeNum(int fm_vtx_num,
                              int to_vtx_num)
{
  int edge_num;
  edge_num = (Row_List->at(fm_vtx_num))->at(to_vtx_num);
  return(edge_num);
}
//================================================
int DirectedGraph::GetEdgeNum(obj_id_type edge_id)
{
  int edge_num = -1;
  for(int i=0; i<Num_Edges; i++)
    {
    if( Edge_List->at(i) == edge_id ) edge_num = i;
    }
  if(edge_num < 0)
    {
    // error: specified vertex not found in graph
    return(-1);
    }
  return(edge_num);
}
//================================================
int DirectedGraph::GetVertexNum(obj_id_type vtx_id)
{
  int vtx_num = -1;
  for(int i=0; i<Num_Verts; i++)
    {
    if( Vertex_List->at(i) == vtx_id ) vtx_num = i;
    }
  if(vtx_num < 0)
    {
    // error: specified vertex not found in graph
    return(-1);
    }
  return(vtx_num);
}
//================================================
obj_id_type DirectedGraph::GetEdgeId(int edge_num)
{
  if( edge_num >= 0 && edge_num < Num_Edges )
    return( Edge_List->at(edge_num) );
  else
    return(NULL);
}
//================================================
obj_id_type DirectedGraph::GetVertexId(int vtx_num)
{
  if( vtx_num >= 0 && vtx_num < Num_Verts )
    return( Vertex_List->at(vtx_num) );
  else
    return(NULL);
}
