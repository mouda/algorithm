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
#include <deque>
#include <iostream>
#include <stdexcept>
#include <assert.h>

#define INFINITY 65536
using namespace std;

enum Color{ WHITE, GRAY, BLACK };

//namespace Graph
//{
  template <class T>
    class graph
    {
      public :
        explicit graph(const vector<pair<T, T> > &vertices, 
            const vector<int> &weight, string N);
        ~graph(){}
        void insert_vertex_pair_by_keys(T key1, T key2, int value);


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
            void connect_edge(vertex *adjacent, int value);
            const T key() const {return m_Key;}
            list<edge> &edges() {return m_Edges;}
            edge *contains_edge_to_vertex_with_key(const T key);
          private:
            list<edge> m_Edges;
            T m_Key;

          public: //data
            Color color;    //BFS DFS
            vertex *pi;
            int distance;

        }; // END VERTEX

        // Private methods and member variables
      private:
        list<vertex> m_Vertices;
      public:
        vertex *contains_vertex(const T key);
        const list<vertex> &vertices() const { return m_Vertices;}
        unsigned BFS( const T &start, vector< pair< T, T> > &tree, 
            vector<int> &value);
        unsigned DFS( const T &start, vector< pair< T, T> > &tree,
            vector<int> &value);
        void DFS_Visit( vertex &u, vector< pair< T, T> > &tree, 
            vector<int> &value);

        unsigned MST( const T &start, vector< pair<T, T> > &tree, 
            vector<int> &value);
        vertex* EXTRACT_MIN( deque<vertex*> & queue );
        void DrawGraph();
        void printGraph();
        bool IsSpanningTree( graph &toBeCompare);

        static bool compare_d( edge j, edge  i){
        return i.m_Edge->key() > j.m_Edge->key();
        }

        string name;
    };

//}


// -------------------------------------------------------------------------- //
// @Description: implemetation
// @Provides: 
// -------------------------------------------------------------------------- //


/*!
 * Constructor of graph: Take a pair of vertices as connection, attempt 
 * to insert if not already in graph. Then connect them in edge list
 */
  template <class T>
graph<T>::graph(const vector<pair<T, T> > &vertices_relation, 
    const vector<int> &weight, string N)
{

#ifdef NDEBUG
  cout << "Inserting pairs: " << endl;
#endif

  typename vector<pair<T, T> >::const_iterator insert_it 
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
    print_it->edges().sort(graph<T>::compare_d);
  }
}

/*!
 * Takes in a value of type T as a key and 
 * inserts it into graph data structure if 
 * key not already present
 */
  template <typename T>
void graph<T>::insert_vertex_pair_by_keys(T key1, T key2, int value)
{
  /*!
   * Check if vertices already in graph
   */
  graph<T>::vertex *insert1 = contains_vertex(key1);
  graph<T>::vertex *insert2 = contains_vertex(key2);
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
    insert2->connect_edge(insert1, value);
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
typename graph<T>::vertex *graph<T>::contains_vertex(T key)
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
void graph<T>::vertex::connect_edge(graph<T>::vertex *adjacent, int value)
{
  if (adjacent == NULL)
    return;

  if (!contains_edge_to_vertex_with_key(adjacent->key())) {
    graph<T>::edge e(adjacent, value);
    m_Edges.push_back(e);
  }
}

/*!
 * Private member function that check if there is already
 * an edge between the two vertices
 */
  template <class T>
typename graph<T>::edge* 
graph<T>::vertex::contains_edge_to_vertex_with_key(const T key)
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

  template <class T>
void graph<T>::printGraph()
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

template <class T> 
unsigned graph<T>::BFS( const T &start, vector< pair<T, T> > &tree, vector<int> &value)
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

  deque<graph<T>::vertex *> queue;
  queue.push_back(&(*s)); //get the pointer from iterator
  graph<T>::vertex *ui;
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
        tree.push_back( pair<T, T>(ui->key(), v->m_Edge->key()));
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

  template <class T> 
unsigned graph<T>::DFS( const T &start, vector< pair< T, T> > &tree, 
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

template <class T>
void graph<T>::DFS_Visit( vertex &u, vector< pair<T ,T> > &tree,
   vector<int> &value )
{
 u.color = GRAY; 
 typename list<edge>::const_iterator v = u.edges().begin();
 for (; v != u.edges().end(); v++) 
   if ( v->m_Edge->color == WHITE) {
     tree.push_back( pair<T,T>(u.key(), v->m_Edge->key()) );
     value.push_back( v->m_Weight);
     DFS_Visit( *v->m_Edge, tree, value ); 
   }
}

// -------------------------------------------------------------------------- //
// @Description: perform the firms algortihm to find the miminum spanning tree
// @Provides: mouda 
// -------------------------------------------------------------------------- //

  template <class T> 
unsigned graph<T>::MST( const T &start, vector< pair< T, T> > &tree,
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

  deque<graph<T>::vertex *> queue;
  graph<T>::vertex *ui;
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
      tree.push_back(pair<T, T>(ui->key(), ui->pi->key()));
      value.push_back(ui->distance);
    }
  }
  return tree.size()+1;
}
  template<class T>
typename graph<T>::vertex* 
graph<T>::EXTRACT_MIN( deque<graph<T>::vertex* > &queue)
{
  typename deque<graph<T>::vertex*>::iterator it = queue.begin();
  typename deque<graph<T>::vertex*>::iterator it_min = queue.begin();
  graph<T>::vertex  *minimum;
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

  template<class T>
bool graph<T>::IsSpanningTree(  graph &toBeCompare)
{
  if (toBeCompare.vertices().size() != this->m_Vertices.size())  return false;

  graph<T>::vertex *rescentVertex = 0;
  graph<T>::edge *rescentEdge = 0;
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



