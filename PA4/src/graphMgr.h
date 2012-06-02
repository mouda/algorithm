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

class graph
{
  public :
    explicit graph(const vector<pair<unsigned, unsigned> > &vertices, 
        const vector<int> &weight, string N);
    ~graph(){}
    void insert_vertex_pair_by_keys(unsigned key1, unsigned key2, int value);


    // Private contained classes
  private:
    // Forward Definition of vertex
    class vertex;

    struct edge
    {
      edge(vertex *edge, int weight) : m_Edge(edge), m_Weight(weight){}
      vertex *m_Edge;
      int m_Weight;
      unsigned flow;
    }; // END EDGE

    class vertex
    {
      public:
        vertex(unsigned key) : m_Key(key) {}
        void connect_edge(vertex *adjacent, int value);
        const unsigned key() const {return m_Key;}
        list<edge> &edges() {return m_Edges;}
        edge *contains_edge_to_vertex_with_key(const unsigned key);
      private:
        list<edge> m_Edges;
        unsigned m_Key;

      public: //data
        Color color;    //BFS DFS
        vertex *pi;
        int distance;

    }; // END VERTEX

    // Private methods and member variables
  private:
    list<vertex> m_Vertices;
  public:
    vertex *contains_vertex(const unsigned key);
    const list<vertex> &vertices() const { return m_Vertices;}
    /* HW3 */
    unsigned BFS( const unsigned &start, 
        vector< pair< unsigned, unsigned> > &tree, vector<int> &value);

    unsigned DFS( const unsigned &start, 
        vector< pair< unsigned, unsigned> > &tree,vector<int> &value);

    void DFS_Visit( vertex &u, vector< pair< unsigned, unsigned> > &tree, 
        vector<int> &value);

    unsigned MST( const unsigned &start, 
        vector< pair<unsigned, unsigned> > &tree, vector<int> &value);

    vertex* EXTRACT_MIN( deque<vertex*> & queue );
    void DrawGraph();
    void printGraph();
    bool IsSpanningTree( graph &toBeCompare);

    /* HW5 */
    unsigned MaxFlow( unsigned source, unsigned sink, 
        vector< pair< unsigned, unsigned > > &tree, vector<int> &value );
    bool IsPathInResidual( vertex *source, vertex *sink, vector<edge*> &path);
    bool DFS_RPath( vertex &u, vertex &sink, vector<edge*> &path, vector<edge*> &trace);

    bool IsFlow( graph &toBeCompare);

    static bool compare_d( edge j, edge  i){
      return i.m_Edge->key() > j.m_Edge->key();
    }
    static bool compare_flow( edge *i, edge *j) {
      return (i->m_Weight - i->flow) > (j->m_Weight - j->flow);
    }
    string name;
};


