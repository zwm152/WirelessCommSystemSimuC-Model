//
//  File = digraph.h
//

#ifndef _DIGRAPH_H_
#define _DIGRAPH_H_

#include "deque"
//#include "list"
#include "vector"
typedef void* obj_id_type;

class DirectedGraph
{
public:
  DirectedGraph( void );
  ~DirectedGraph(void);
  int GetNumVerts(void);
  int GetNumEdges(void);
  int AddVertex(obj_id_type vertex_id);
  int AddEdge(  obj_id_type edge_id,
                int fm_vtx_num,
                int to_vtx_num);
  int AddEdge(  obj_id_type edge_id,
                obj_id_type fm_vtx_id,
                obj_id_type to_vtx_id);

  int GetEdgeNum( obj_id_type edge_id );
  int GetEdgeNum( int fm_vtx_num,
                  int to_vtx_num);

  int GetVertexNum(obj_id_type vtx_id);
  obj_id_type GetEdgeId(int edge_num);
  obj_id_type GetVertexId(int vtx_num);
protected:
//  std::list<std::vector<int>> *Row_List;
  std::deque<std::vector<int>* > *Row_List;
  std::vector<obj_id_type> *Vertex_List;
  std::vector<obj_id_type> *Edge_List;
  int Num_Verts;
  int Num_Edges;

private:
  int Default_Val;
  
};

#endif //_DIGRAPH_H_
