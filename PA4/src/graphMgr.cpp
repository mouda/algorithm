// **************************************************************************
// File       [ graphMgr.cpp ]
// Author     [ mouda ]
// Synopsis   [ ]
// Date       [ 2012/05/7 created ]
// **************************************************************************

#include "graphMgr.h"
#include <list>
#include <set>
#include <algorithm>

using namespace std;


// -------------------------------------------------------------------------- //
// @Description: implemetation
// @Provides: 
// -------------------------------------------------------------------------- //


/*!
 * Constructor of graph: Take a pair of vertices as connection, attempt 
 * to insert if not already in graph. Then connect them in edge list
 */
  
graph::graph(const vector<pair<unsigned, unsigned> > &vertices_relation, 
    const vector<int> &weight, string N)
{

#ifdef NDEBUG
  cout << "Inserting pairs: " << endl;
#endif

  typename vector<pair<unsigned, unsigned> >::const_iterator insert_it 
    = vertices_relation.begin();


  int i = 0; 
  for(; insert_it != vertices_relation.end(); ++insert_it) {

#ifdef NDEBUG
    cout << insert_it->first << " -- > " << insert_it->second 
      << ' '<< weight[i] <<endl;
#endif

    insert_vertex_pair_by_keys(insert_it->first, insert_it->second, weight[i]);
    i++;
  }

  name = N;
  
  typename list<vertex>::iterator print_it = m_Vertices.begin();
  for(; print_it != m_Vertices.end(); ++print_it) {
    print_it->edges().sort(graph::compare_d);
  }
}

/*!
 * Takes in a value of type unsigned as a key and 
 * inserts it into graph data structure if 
 * key not already present
 */
  
void graph::insert_vertex_pair_by_keys(unsigned key1, unsigned key2, int value)
{
  /*!
   * Check if vertices already in graph
   */
  graph::vertex *insert1 = contains_vertex(key1);
  graph::vertex *insert2 = contains_vertex(key2);
  /*!
   * If not in graph then insert it and get a pointer to it
   * to pass into edge. See () for information on how
   * to build graph
   */ 
  if (insert1 == NULL) {
    m_Vertices.push_back(vertex(key1));
    insert1 = contains_vertex(key1);
  }
  if (insert2 == NULL) {
    m_Vertices.push_back(vertex(key2));
    insert2 = contains_vertex(key2);
  }

#ifdef NDEBUG
  assert(insert1 != NULL && "Failed to insert first vertex");
  assert(insert2 != NULL && "Failed to insert second vertex");
#endif

  /*!
   * At this point we should have a vertex to insert an edge on
   * if not throw an error.
   */ 
  if (insert1 != NULL && insert2 != NULL) {
    insert1->connect_edge(insert2, value);
//    insert2->connect_edge(insert1, value); //uni-directed
  } else {
    throw runtime_error("Unknown");
  }
}

/*!
 * Search the std::list of vertices for key
 * if present return the vertex to indicate
 * already in graph else return NULL to indicate
 * new node
 */
  
typename graph::vertex *graph::contains_vertex(unsigned key)
{
  typename list<vertex >::iterator find_it = m_Vertices.begin();
  for(; find_it != m_Vertices.end(); ++find_it) {
    if (find_it->key() == key) {
      return &(*find_it);
    }
  }
  return NULL;
}

/*!
 * Take the oposing vertex from input and insert it
 * into adjacent list, you can have multiple edges
 * between vertices
 */
  
void graph::vertex::connect_edge(graph::vertex *adjacent, int value)
{
  if (adjacent == NULL)
    return;

  if (!contains_edge_to_vertex_with_key(adjacent->key())) {
    graph::edge e(adjacent, value);
    m_Edges.push_back(e);
  }
}

/*!
 * Private member function that check if there is already
 * an edge between the two vertices
 */
  
typename graph::edge* 
graph::vertex::contains_edge_to_vertex_with_key(const unsigned key)
{
  typename list<edge>::iterator find_it = m_Edges.begin();
  for(; find_it != m_Edges.end(); ++find_it) {
    if (find_it->m_Edge->key() == key) {
      return &(*find_it);
    }   
  }
  return 0;
}

// -------------------------------------------------------------------------- //
// @Description: report all the tree node
// @Provides: mouda 
// -------------------------------------------------------------------------- //

  
void graph::printGraph()
{
  cout << "Printing results: " << endl;
  typename list<vertex>::iterator print_it = m_Vertices.begin();
  for(; print_it != m_Vertices.end(); ++print_it) {
    cout << print_it->key();
    typename list<edge>::const_iterator edge_it 
      = print_it->edges().begin();
    for(; edge_it != print_it->edges().end(); ++edge_it) {
      cout << "-->" << edge_it->m_Edge->key();
    }
    cout << endl;
  }
}

// -------------------------------------------------------------------------- //
// @Description: perform the breath first traversal
// @Provides: mouda
// -------------------------------------------------------------------------- //

 
unsigned 
graph::BFS( const unsigned &start, vector< pair<unsigned, unsigned> > &tree, 
    vector<int> &value)
{
  unsigned vertexNumber = 0;
  typename list<vertex>::iterator s = m_Vertices.begin();
  for (;  s != m_Vertices.end(); s++)
    if ( s->key() == start ) break; 
  if ( s == m_Vertices.end()) return vertexNumber; 

  typename list<vertex>::iterator u = m_Vertices.begin();
  for (; u != m_Vertices.end(); u++) {
    u->color = WHITE; 
    u->distance = -1; 
    u->pi = 0; 
  }
  s->color = GRAY;
  s->distance = 0;
  s->pi = 0;

  deque<graph::vertex *> queue;
  queue.push_back(&(*s)); //get the pointer from iterator
  graph::vertex *ui;
  while ( !queue.empty() ){
    ui = queue[queue.size()-1];
    queue.pop_back();
    typename list<edge>::const_iterator v = ui->edges().begin();
    for(; v != ui->edges().end(); ++v) {
      if ( v->m_Edge->color == WHITE ){ 
        v->m_Edge->color = GRAY;
        v->m_Edge->distance = ui->distance + 1;
        v->m_Edge->pi = ui;
        queue.push_front(v->m_Edge);
        //record
        tree.push_back( pair<unsigned, unsigned>(ui->key(), v->m_Edge->key()));
        value.push_back(v->m_Weight);
      }
    }
    ui->color = BLACK;
    vertexNumber++;
  }


  return vertexNumber;

#define INF 65536
}

// -------------------------------------------------------------------------- //
// @Description: perform the depth first traversal
// @Provides: mouda 
// -------------------------------------------------------------------------- //

   
unsigned graph::DFS( const unsigned &start, vector< pair< unsigned, unsigned> > &tree, 
    vector<int> &value)
{
  typename list<vertex>::iterator s = m_Vertices.begin();
  for (;  s != m_Vertices.end(); s++)
    if ( s->key() == start ) break; 
  if ( s == m_Vertices.end()) return 0; 

  typename list<vertex>::iterator u = m_Vertices.begin();
  for (; u != m_Vertices.end(); u++) {
    u->color = WHITE; 
    u->pi = 0; 
  }


  if (s->color == WHITE)  DFS_Visit( *s , tree, value);


  return tree.size()+1;
}


void graph::DFS_Visit( vertex &u, vector< pair<unsigned ,unsigned> > &tree,
   vector<int> &value )
{
 u.color = GRAY; 
 typename list<edge>::const_iterator v = u.edges().begin();
 for (; v != u.edges().end(); v++) 
   if ( v->m_Edge->color == WHITE) {
     tree.push_back( pair<unsigned,unsigned>(u.key(), v->m_Edge->key()) );
     value.push_back( v->m_Weight);
     DFS_Visit( *v->m_Edge, tree, value ); 
   }
}

// -------------------------------------------------------------------------- //
// @Description: perform the firms algortihm to find the miminum spanning tree
// @Provides: mouda 
// -------------------------------------------------------------------------- //

   
unsigned graph::MST( const unsigned &start, vector< pair< unsigned, unsigned> > &tree,
    vector<int> &value)
{
  typename list<vertex>::iterator r = m_Vertices.begin();
  for (;  r != m_Vertices.end(); r++)
    if ( r->key() == start ) break; 
  if ( r == m_Vertices.end()) return 0; 

  typename list<vertex>::iterator u = m_Vertices.begin();
  for (; u != m_Vertices.end(); u++) {
    u->distance = INFINITY;
    u->pi = 0; 
  }
  r->distance = 0;

  deque<graph::vertex *> queue;
  graph::vertex *ui;
  typename list<edge>::const_iterator v ;

  for (u = m_Vertices.begin(); u != m_Vertices.end(); u++) 
    queue.push_back(&(*u));

  while( !queue.empty() ) {
    ui = EXTRACT_MIN( queue );
    v = ui->edges().begin();
    for(; v != ui->edges().end(); ++v) {
      if ( (find(queue.begin(),queue.end(), v->m_Edge) != queue.end()) && 
          (v->m_Weight < v->m_Edge->distance)){ 
        v->m_Edge->pi = ui;
        v->m_Edge->distance = v->m_Weight;
      }
    }
    if (ui->pi != 0) {
      tree.push_back(pair<unsigned, unsigned>(ui->key(), ui->pi->key()));
      value.push_back(ui->distance);
    }
  }
  return tree.size()+1;
}
  
typename graph::vertex* 
graph::EXTRACT_MIN( deque<graph::vertex* > &queue)
{
  typename deque<graph::vertex*>::iterator it = queue.begin();
  typename deque<graph::vertex*>::iterator it_min = queue.begin();
  graph::vertex  *minimum;
  for (; it != queue.end(); it++) {
    if (((*it)->distance) < ((*it_min)->distance)) 
      it_min = it;
  }
  minimum = *it_min;
  queue.erase(it_min);
  return minimum;
} 

// -------------------------------------------------------------------------- //
// @Description: Is spanning tree, need to check out two things, we have to 
//  compare the number of vertices in the original graph and the input graph.
//  Second, if one edge exist in the input graph, there must exist edge with 
//  the same weight.
// @Provides: mouda 
// -------------------------------------------------------------------------- //

  
bool graph::IsSpanningTree(  graph &toBeCompare)
{
  if (toBeCompare.vertices().size() != this->m_Vertices.size())  return false;

  graph::vertex *rescentVertex = 0;
  graph::edge *rescentEdge = 0;
  typename list<vertex>::iterator print_it = m_Vertices.begin();
  for(; print_it != m_Vertices.end(); ++print_it) {
   rescentVertex = toBeCompare.contains_vertex(print_it->key());
   if (!rescentVertex) return false;
    
    typename list<edge>::iterator edge_it 
      = print_it->edges().begin();
    for(; edge_it != print_it->edges().end(); ++edge_it) {
      rescentEdge = 
        rescentVertex->contains_edge_to_vertex_with_key(edge_it->m_Edge->key());
      if ( (!rescentEdge) || (rescentEdge->m_Weight != edge_it->m_Weight))
      return false;
    }
  }
  return true;
}



