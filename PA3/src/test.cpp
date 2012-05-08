// **************************************************************************
// File       [ test.cpp ]
// Author     [ mouda ]
// Synopsis   [ ]
// Date       [ 2012/05/7 created ]
// **************************************************************************
//

#include "graphMgr.h"
#include <cstdlib>

int main(int argc, char *argv[])
{
  std::vector<std::pair<int, int> > graph_vect;
  for (int i = 0; i < 100; i++) {
    graph_vect.push_back(std::pair<int, int>(rand()%20, rand()%20));
  }
  Graph::graph<int> my_graph(graph_vect);
  return 0;
}
