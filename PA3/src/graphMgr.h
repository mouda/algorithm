// **************************************************************************
// File       [ graphMgr.h ]
// Author     [ mouda ]
// Synopsis   [ ]
// Date       [ 2012/05/7 created ]
// **************************************************************************

#include <list>
#include <set>

  class Graph {
    public:
      Graph() {}

      class Arc;
      class Node {
        friend class std::list< Graph::Node >;
        friend class Graph;
        Node( Graph & g ) : g_( g ) {}
        public:
        ~Node();
        void destroy();// { g_.nodes_.erase( *this ); }
        bool operator==( const  Node compare );
        std::set< Arc * > outgoing_;
        std::set< Arc * > incoming_;
        Graph & g_;
        Graph::Arc * newArcTo( Node * );
      };
      
      class Arc {
        friend class std::list< Graph::Arc >;
        friend class Graph;
        Arc( Graph & g ) : g_( g ), from_( 0 ), to_( 0 ) {}
        public:
        ~Arc();
        void destroy();// { g_.arcs_.erase( *this ); }
        bool operator==( const  Arc compare );
        std::set< Arc * > outgoing_;
        Graph & g_;
        Graph::Node * from_;
        Graph::Node * to_;
      };

      std::list< Node > nodes_;
      std::list< Arc > arcs_;

      Node * newNode();
      Arc * newArc();
  };
