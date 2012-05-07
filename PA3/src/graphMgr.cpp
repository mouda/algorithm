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
// @Description: Node implementation
// @Provides: 
// -------------------------------------------------------------------------- //

Graph::Node::~Node() 
{
  while( !outgoing_.empty() ) {
    delete *outgoing_.begin();
  }
}

Graph::Arc * Graph::Node::newArcTo( Node * n ) 
{
  Graph::Arc * a = g_.newArc();
  a->to_ = n;
  n->incoming_.insert( a );
  a->from_ = this;
  outgoing_.insert( a );
}

bool Graph::Node::operator==(  const Node compare) {
  if ( *this == compare) {
    return true;
  }
  else return false;

}

void Graph::Node::destroy()
{
  list< Node >::iterator it; 
  it = find( g_.nodes_.begin(), g_.nodes_.end(), *this);
  g_.nodes_.erase(it);
}

// -------------------------------------------------------------------------- //
// @Description: arc implementation
// @Provides: 
// -------------------------------------------------------------------------- //

Graph::Arc::~Arc() 
{
  if( from_ ) {
    from_->outgoing_.erase( this );
    to_->incoming_.erase( this );
  }
}

bool Graph::Arc::operator==(  const Arc compare) {
  if ( *this == compare) {
    return true;
  }
  else return false;

}

void Graph::Arc::destroy()
{
  list< Arc >::iterator it; 
  it = find( g_.arcs_.begin(), g_.arcs_.end(), *this);
  g_.arcs_.erase(it);
}


// -------------------------------------------------------------------------- //
// @Description: graph implementation
// @Provides: 
// -------------------------------------------------------------------------- //


Graph::Node * Graph::newNode() 
{
  nodes_.push_back( Graph::Node( *this ) );
  return &nodes_.back();
}

Graph::Arc * Graph::newArc() 
{
  arcs_.push_back( Graph::Arc( *this ) );
  return &arcs_.back();
}

