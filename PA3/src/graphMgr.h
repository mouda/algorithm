// **************************************************************************
// File       [ graphMgr.h ]
// Author     [ mouda ]
// Synopsis   [ ]
// Date       [ 2012/05/7 created ]
// **************************************************************************


#include <list>
#include <algorithm>
#include <vector>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <assert.h>

using namespace std;

namespace Graph
{
  template <class T>
  class graph
  {
  public :
    explicit graph(const vector<pair<T, T> > &vertices, const vector<int> &weight);
    ~graph(){}
    void insert_vertex_pair_by_keys(T key1, T key2);

  // Private contained classes
  private:
   // Forward Definition of vertex
   class vertex;

    struct edge
    {
      edge(vertex *edge, int weight) : m_Edge(edge), m_Weight(weight){}
      vertex *m_Edge;
      int m_Weight;
    }; // END EDGE

    class vertex
    {
    public:
      vertex(T key) : m_Key(key) {}
      void connect_edge(vertex *adjacent);
      const T key() const {return m_Key;}
      const list<edge> &edges() const {return m_Edges;}
    private:
      list<edge> m_Edges;
      T m_Key;
      bool contains_edge_to_vertex_with_key(const T key);
    }; // END VERTEX

   // Private methods and member variables
   private:
     list<vertex> m_Vertices;
     vertex *contains_vertex(const T key);
     string name;
  };
}

/*!
 * Constructor of graph: Take a pair of vertices as connection, attempt 
 * to insert if not already in graph. Then connect them in edge list
 */
template <class T>
Graph::graph<T>::graph(const vector<pair<T, T> > &vertices_relation, const vector<int> &weight)
{

#ifndef NDEBUG
  cout << "Inserting pairs: " << endl;
#endif

  typename vector<pair<T, T> >::const_iterator insert_it 
    = vertices_relation.begin();

  for(; insert_it != vertices_relation.end(); ++insert_it) {

#ifndef NDEBUG
    cout << insert_it->first << " -- > " << insert_it->second 
      << endl;
#endif
    
    insert_vertex_pair_by_keys(insert_it->first, insert_it->second);
  }

#ifndef NDEBUG
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
#endif

}

/*!
 * Takes in a value of type T as a key and 
 * inserts it into graph data structure if 
 * key not already present
 */
template <typename T>
void Graph::graph<T>::insert_vertex_pair_by_keys(T key1, T key2)
{
  /*!
   * Check if vertices already in graph
   */
  Graph::graph<T>::vertex *insert1 = contains_vertex(key1);
  Graph::graph<T>::vertex *insert2 = contains_vertex(key2);
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

#ifndef NDEBUG
    assert(insert1 != NULL && "Failed to insert first vertex");
    assert(insert2 != NULL && "Failed to insert second vertex");
#endif

  /*!
   * At this point we should have a vertex to insert an edge on
   * if not throw an error.
   */ 
  if (insert1 != NULL && insert2 != NULL) {
    insert1->connect_edge(insert2);
    insert2->connect_edge(insert1);
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
template <typename T>
typename Graph::graph<T>::vertex *Graph::graph<T>::contains_vertex(T key)
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
template <class T>
void Graph::graph<T>::vertex::connect_edge(Graph::graph<T>::vertex *adjacent)
{
  if (adjacent == NULL)
    return;

  if (!contains_edge_to_vertex_with_key(adjacent->key())) {
    Graph::graph<T>::edge e(adjacent, 1);
    m_Edges.push_back(e);
  }
}

/*!
 * Private member function that check if there is already
 * an edge between the two vertices
 */
template <class T>
bool Graph::graph<T>::vertex::contains_edge_to_vertex_with_key(const T key)
{
  typename list<edge>::iterator find_it = m_Edges.begin();
  for(; find_it != m_Edges.end(); ++find_it) {
    if (find_it->m_Edge->key() == key) {
      return true;
    }   
  }
  return false;
}
