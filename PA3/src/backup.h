
namespace graphMgr {


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

Graph::Arc::~Arc() 
{
  if( from_ ) {
    from_->outgoing_.erase( this );
    to_->incoming_.erase( this );
  }
}

Graph::Node * Graph::newNode() 
{
  nodes_.push_back( Graph::Node( this ) );
  return &nodes_.back();
}

Graph::Node * Graph::newArc() 
{
  nodes_.push_back( Graph::Node( this ) );
  return &nodes_.back();
}

};

