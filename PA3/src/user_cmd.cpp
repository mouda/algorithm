// **************************************************************************
// File       [ test_cmd.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#include "user_cmd.h"
#include "graphMgr.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace CommonNs;

Graph::graph<string> *my_graph = 0;

TestCmd::TestCmd(const char * const name) : Cmd(name) {
  optMgr_.setShortDes("test");
  optMgr_.setDes("test");

  Opt *opt = new Opt(Opt::BOOL, "print usage", "");
  opt->addFlag("h");
  opt->addFlag("help");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "print the string of -s", "[string]");
  opt->addFlag("s");
  optMgr_.regOpt(opt);
}

TestCmd::~TestCmd() {}

bool TestCmd::exec(int argc, char **argv) {
  optMgr_.parse(argc, argv);

  if (optMgr_.getParsedOpt("h")) {
    optMgr_.usage();
    return true;
  }

  if (optMgr_.getParsedOpt("s")) {
    printf("%s\n", optMgr_.getParsedValue("s"));
    return true;
  }
  else
    printf("hello world !!\n");


  return true;
}

// -------------------------------------------------------------------------- //
// @Description: Read graph
// @Provides: 
// -------------------------------------------------------------------------- //

ReadGraph::ReadGraph(const char * const name) : Cmd(name)
{
  optMgr_.setShortDes("Read the graph in dot format");
  optMgr_.setDes("Read the graph in dot format");
  Opt *opt = new Opt(Opt::BOOL, "print usage", "");
  opt->addFlag("h");
  opt->addFlag("help");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "dot_filename", "<dot_filename>");
  //  opt->addFlag("s");
  optMgr_.regOpt(opt);

}

ReadGraph::~ReadGraph(){}

bool ReadGraph::exec(int argc, char **argv)
{
  optMgr_.parse(argc, argv);
  if ( optMgr_.getParsedOpt("h"))
  {
    optMgr_.usage();
    return true;
  }

  if (argc < 2) {
    fprintf(stderr, "**ERROR ReadGraph::exec(): ");
    fprintf(stderr, "please specify source file\n");
    return false;
  }

  ifstream inFile;
  inFile.open(argv[1]);
  if (inFile.fail()) {
    fprintf(stderr, "**ERROR ReadGraph::exec(): ");
    fprintf(stderr, "file cannot be opened\n");
    return false;
  }

  if (argc > 2) {
    fprintf(stderr, "**ERROR ReadGraph::exec(): ");
    fprintf(stderr, "too many arguments\n");
    return false;
  }

  stringstream ss;
  string s, token1, ignore ,token2, graphName, number ;
  size_t found1, found2;
  int weight = 0;  

  getline(inFile,s);
  ss.str(s);
  ss >> token1 >> graphName; 
 
  vector< pair<string, string > > graph_vect;  
  vector< int > graph_weight;

  while( getline(inFile,s) ){

    ss.str(s);
    ss >> token1; 
    if (!token1.compare("}")) break; 
    ss >> ignore >> token2; 
    graph_vect.push_back( pair<string, string>( token1, token2 ) ); 

    found1 = s.find_first_of('"');
    found2 = s.find_last_of('"');
    number = s.substr(found1+1, found2-found1-1);
    graph_weight.push_back(atoi(number.c_str()));

    s.clear();
  }
  
   my_graph = new Graph::graph<string>(graph_vect, graph_weight, graphName);


  inFile.close();
  return true;

}

// -------------------------------------------------------------------------- //
// @Description: write tree dfs
// @Provides: 
// -------------------------------------------------------------------------- //

WriteTreeDfs::WriteTreeDfs(const char * const name) : Cmd(name)
{
  optMgr_.setShortDes("Perform DFS and write to a dot file.");
  optMgr_.setDes("Perform depth first search starting from source node. Then write to a dot file.");
  Opt *opt = new Opt(Opt::BOOL, "print usage", "");
  opt->addFlag("h");
  opt->addFlag("help");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "source node", "<sourcenode>");
  opt->addFlag("s");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "dot file", "<dot_filename>");
  opt->addFlag("o");
  optMgr_.regOpt(opt);

}

WriteTreeDfs::~WriteTreeDfs(){}

bool WriteTreeDfs::exec(int argc, char **argv)
{
  optMgr_.parse(argc, argv);
  if ( optMgr_.getParsedOpt("h"))
  {
    optMgr_.usage();
    return true;
  }
  char *sname, *fname;
  if (optMgr_.getParsedOpt("s"))
    sname = optMgr_.getParsedValue("s");
  else
  {
    fprintf(stderr, "**ERROR WriteTreeDfs::exec(): input node # is needed\n");
    return false;
  }

  if (optMgr_.getParsedOpt("o")) fname = optMgr_.getParsedValue("o");
  else
  {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): output dot file path is needed\n");
    return false;
  }

  //TODO
  if (my_graph == 0 ) 
  {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): read dot file is needed first\n");
    return false;
  }
  my_graph->printGraph();
}


// -------------------------------------------------------------------------- //
// @Description: write tree bfs
// @Provides: 
// -------------------------------------------------------------------------- //

WriteTreeBfs::WriteTreeBfs(const char * const name) : Cmd(name)
{
  optMgr_.setShortDes("Perform BFS and write to a dot file.");
  optMgr_.setDes("Perform breath first search starting from source node. Then write to a dot file.");
  Opt *opt = new Opt(Opt::BOOL, "print usage", "");
  opt->addFlag("h");
  opt->addFlag("help");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "source node", "<sourcenode>");
  opt->addFlag("s");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "dot file", "<dot_filename>");
  opt->addFlag("o");
  optMgr_.regOpt(opt);

}

WriteTreeBfs::~WriteTreeBfs(){}

bool WriteTreeBfs::exec(int argc, char **argv)
{
  optMgr_.parse(argc, argv);
  if ( optMgr_.getParsedOpt("h"))
  {
    optMgr_.usage();
    return true;
  }
  char *sname, *fname;
  if (optMgr_.getParsedOpt("s"))
    sname = optMgr_.getParsedValue("s");
  else
  {
    fprintf(stderr, "**ERROR WriteTreeBfs::exec(): input node # is needed\n");
    return false;
  }

  if (optMgr_.getParsedOpt("o")) fname = optMgr_.getParsedValue("o");
  else
  {
    fprintf(stderr,
        "**ERROR WriteTreeBfs::exec(): output dot file path is needed\n");
    return false;
  }

  if (my_graph == 0 ) 
  {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): read dot file is needed first\n");
    return false;
  }
  //TODO
  ofstream outFile;
  string startVetex;
  vector< pair<string,string> > result;
  startVetex = sname;

  outFile.open(fname);
  my_graph->printGraph();

  outFile << my_graph->name <<'{' <<endl; 
  if ( my_graph->BFS(&startVetex, result) ) {
    for (size_t i = 0; i < result.size(); i++) {
      cout << result[i].first << ' ' << result[i].second << endl;
    }
  } else {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): input node# doesn't exist! \n");
    return false;
  }
  

//  cout <<" the result lenght " << result.size() << endl;

  /* write the bfs tree to the output file */

}

// -------------------------------------------------------------------------- //
// @Description: write tree mst
// @Provides: 
// -------------------------------------------------------------------------- //

WriteTreeMst::WriteTreeMst(const char * const name) : Cmd(name)
{
  optMgr_.setShortDes("Perform Prim's MST  and write to a dot file.");
  optMgr_.setDes("Perform Prim’s MST starting from the root node.    Then write to a dot file.");
  Opt *opt = new Opt(Opt::BOOL, "print usage", "");
  opt->addFlag("h");
  opt->addFlag("help");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "algorithm", "<algorithm>");
  opt->addFlag("a");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "rootnode", "<rootnode>");
  opt->addFlag("r");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "dot file", "<dot_filename>");
  opt->addFlag("o");
  optMgr_.regOpt(opt);

}

WriteTreeMst::~WriteTreeMst(){}

bool WriteTreeMst::exec(int argc, char **argv)
{
  optMgr_.parse(argc, argv);
  if ( optMgr_.getParsedOpt("h"))
  {
    optMgr_.usage();
    return true;
  }
  char *sname, *fname;
  if (optMgr_.getParsedOpt("s"))
    sname = optMgr_.getParsedValue("s");
  else
  {
    fprintf(stderr, "**ERROR WriteTreeMst::exec(): input node # is needed\n");
    return false;
  }

  if (optMgr_.getParsedOpt("o")) fname = optMgr_.getParsedValue("o");
  else
  {
    fprintf(stderr, 
        "**ERROR WriteMSTCmd::exec(): output dot file path is needed\n");
    return false;
  }

  //TODO


}

// -------------------------------------------------------------------------- //
// @Description: verify if the graph is spanning tree
// @Provides: 
// -------------------------------------------------------------------------- //

IsSpanningTree::IsSpanningTree(const char * const name) : Cmd(name)
{
  optMgr_.setShortDes("Check if the dot file is a spanning tree");
  optMgr_.setDes("Check if the dot file is a spanning tree of the existing graph. The output is simply Yes or No output to the screen.  No output file is needed.");
  Opt *opt = new Opt(Opt::BOOL, "print usage", "");
  opt->addFlag("h");
  opt->addFlag("help");
  optMgr_.regOpt(opt);


  opt = new Opt(Opt::STR_REQ, "dot file", "<dot_filename>");
  opt->addFlag("i");
  optMgr_.regOpt(opt);

}

IsSpanningTree::~IsSpanningTree(){}

bool IsSpanningTree::exec(int argc, char **argv)
{
  optMgr_.parse(argc, argv);
  if ( optMgr_.getParsedOpt("h"))
  {
    optMgr_.usage();
    return true;
  }
  char *sname, *fname;
  if (optMgr_.getParsedOpt("s"))
    sname = optMgr_.getParsedValue("s");
  else
  {
    fprintf(stderr, "**ERROR IsSpanningTree::exec(): input node # is needed\n");
    return false;
  }

  if (optMgr_.getParsedOpt("o")) fname = optMgr_.getParsedValue("o");
  else
  {
    fprintf(stderr, 
        "**ERROR IsSpanningTree::exec(): output dot file path is needed\n");
    return false;
  }

  //TODO
}
