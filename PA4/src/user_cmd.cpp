// **************************************************************************
// File       [ test_cmd.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#include "user_cmd.h"
#include "graphMgr.h"
#include "tm_usage.h"
#include "utility.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;
using namespace CommonNs;

graph *my_graph = 0;

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

  vector< pair<unsigned, unsigned > > graph_vect;  
  vector< int > graph_weight;
  unsigned Nvalue;

  while( getline(inFile,s) ){

    ss.str(s);
    ss >> token1; 
    if (!token1.compare("}")) break; 
    ss >> ignore >> token2; 
    token1 = token1.substr(1);
    token2 = token2.substr(1);
    graph_vect.push_back( pair<unsigned, unsigned>( 
          strtol(token1.c_str(),NULL,0),strtol(token2.c_str(),NULL,0) ) 
        ); 

    found1 = s.find_first_of('"');
    found2 = s.find_last_of('"');
    number = s.substr(found1+1, found2-found1-1);
    graph_weight.push_back(atoi(number.c_str()));

    s.clear();
  }


  if (my_graph == 0) {
    my_graph = new graph(graph_vect, graph_weight, graphName);
  }
  else {
    delete my_graph;
    my_graph = new graph(graph_vect, graph_weight, graphName);
  }
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

#ifdef _TIME_ON_ 
  timeval tvS, tvE;
  CommonNs::TmUsage tmusg;
  CommonNs::TmStat stat;
  tmusg.periodStart();
  gettimeofday( &tvS, NULL);
#endif 

  ofstream outFile;
  string startNode = sname;
  unsigned vertexValue, verticesNum;
  vector< pair<unsigned,unsigned> > result;
  vector< int > value;
  startNode = startNode.substr(1);


  outFile.open(fname);

  verticesNum = my_graph->DFS( strtol(startNode.c_str(),NULL,0), result,value ); 
  if ( verticesNum ) {
#ifdef _TIME_ON_ 
    gettimeofday( &tvE, NULL);
    tmusg.getTotalUsage(stat);
    writeMessage(outFile, 
        value, 
        result, 
        my_graph->name,
        verticesNum,
        (double)(1000000*(tvE.tv_sec-tvS.tv_sec)+tvE.tv_usec-tvS.tv_usec)/1000000, 
        stat.vmPeak / 1024.0
        );
#endif 
    outFile.close();
  } else {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): input node# doesn't exist! \n");
    outFile.close();
    return false;
  }
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

#ifdef _TIME_ON_ 
  timeval tvS, tvE;
  CommonNs::TmUsage tmusg;
  CommonNs::TmStat stat;
  tmusg.periodStart();
  gettimeofday( &tvS, NULL);
#endif 

  ofstream outFile;
  string startNode = sname;
  unsigned vertexValue, verticesNum;
  vector< pair<unsigned,unsigned> > result;
  vector< int > value;
  startNode = startNode.substr(1);

  outFile.open(fname);

  verticesNum = my_graph->BFS( strtol(startNode.c_str(),NULL,0), result,value ); 
  if ( verticesNum ) {
#ifdef _TIME_ON_ 
    gettimeofday( &tvE, NULL);
    tmusg.getTotalUsage(stat);
    writeMessage(outFile, 
        value, 
        result, 
        my_graph->name,
        verticesNum,
        (double)(1000000*(tvE.tv_sec-tvS.tv_sec)+tvE.tv_usec-tvS.tv_usec)/1000000, 
        stat.vmPeak / 1024.0
        );
#endif 
    outFile.close();
  } else {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): input node# doesn't exist! \n");
    outFile.close();
    return false;
  }



  /* write the bfs tree to the output file */

}

// -------------------------------------------------------------------------- //
// @Description: write tree mst
// @Provides: 
// -------------------------------------------------------------------------- //

WriteTreeMst::WriteTreeMst(const char * const name) : Cmd(name)
{
  optMgr_.setShortDes("Perform Prim's MST  and write to a dot file.");
  optMgr_.setDes("Perform Prim’s MST starting from the root node. \
      Then write to a dot file.");
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
  char *rname, *fname, *algoName;
  if (optMgr_.getParsedOpt("r"))
    rname = optMgr_.getParsedValue("r");
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

  if (optMgr_.getParsedOpt("a"))
    algoName = optMgr_.getParsedValue("a");
  else
  {
    fprintf(stderr, "**ERROR WriteTreeMst::exec(): specisfy algorithm is \
        ineeded\n");
    return false;
  }

  //TODO

#ifdef _TIME_ON_ 
  timeval tvS, tvE;
  CommonNs::TmUsage tmusg;
  CommonNs::TmStat stat;
  tmusg.periodStart();
  gettimeofday( &tvS, NULL);
#endif 

  ofstream outFile;
  string startNode = rname;
  unsigned vertexValue, verticesNum;
  vector< pair<unsigned,unsigned> > result;
  vector< int > value;
  startNode = startNode.substr(1);


  outFile.open(fname);

  verticesNum = my_graph->MST( strtol(startNode.c_str(),NULL,0), result,value ); 
  if ( verticesNum ) {
#ifdef _TIME_ON_ 
    gettimeofday( &tvE, NULL);
    tmusg.getTotalUsage(stat);
    writeMessage(outFile, 
        value, 
        result, 
        my_graph->name,
        verticesNum,
        (double)(1000000*(tvE.tv_sec-tvS.tv_sec)+tvE.tv_usec-tvS.tv_usec)/1000000, 
        stat.vmPeak / 1024.0
        );
#endif 
    outFile.close();
  } else {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): input node# doesn't exist! \n");
    outFile.close();
    return false;
  }

}

// -------------------------------------------------------------------------- //
// @Description: verify if the graph is spanning tree
// @Provides: 
// -------------------------------------------------------------------------- //

IsSpanningTree::IsSpanningTree(const char * const name) : Cmd(name)
{
  optMgr_.setShortDes("Check if the dot file is a spanning tree");
  optMgr_.setDes("Check if the dot file is a spanning tree of the existing \
      graph. The output is simply Yes or No output to the screen. \
      No output file is needed.");
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
  char *iname;
  if (optMgr_.getParsedOpt("i")) iname = optMgr_.getParsedValue("i");
  else
  {
    fprintf(stderr, 
        "**ERROR IsSpanningTree::exec(): output dot file path is needed\n");
    return false;
  }

  if (my_graph == 0 ) 
  {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): read dot file is needed first\n");
    return false;
  }
  //TODO

  ifstream inFile;
  inFile.open(iname);

  stringstream ss;
  string s, token1, ignore ,token2, graphName, number ;
  size_t found1, found2;
  int weight = 0;  

  getline(inFile,s);
  ss.str(s);
  ss >> token1 >> graphName; 

  vector< pair<unsigned, unsigned > > graph_vect;  
  vector< int > graph_weight;
  unsigned Nvalue;

  while( getline(inFile,s) ){

    ss.str(s);
    ss >> token1; 
    if (!token1.compare("}")) break; 
    ss >> ignore >> token2; 
    token1 = token1.substr(1);
    token2 = token2.substr(1);
    graph_vect.push_back( pair<unsigned, unsigned>( 
          strtol(token1.c_str(),NULL,0), strtol(token2.c_str(),NULL,0) ) 
        ); 
    found1 = s.find_first_of('"');
    found2 = s.find_last_of('"');
    number = s.substr(found1+1, found2-found1-1);
    graph_weight.push_back(atoi(number.c_str()));

    s.clear();
  }

  graph toComp(graph_vect, graph_weight, graphName);

  if ( toComp.IsSpanningTree(*my_graph) ) 
    cout << "Yes" << endl; 
  else 
    cout << "NO" << endl;

  inFile.close();
}
// -------------------------------------------------------------------------- //
// @Description: max flow
// @Provides: mouda 
// -------------------------------------------------------------------------- //


WriteMaxFlow::WriteMaxFlow(const char * const name) : Cmd(name)
{
  optMgr_.setShortDes("Check if the dot file is a spanning tree");
  optMgr_.setDes("Check if the dot file is a spanning tree of the existing \
      graph. The output is simply Yes or No output to the screen. \
      No output file is needed.");
  Opt *opt = new Opt(Opt::BOOL, "print usage", "");
  opt->addFlag("h");
  opt->addFlag("help");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "sourcenode", "<sourcenode>");
  opt->addFlag("s");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "sinknode", "<sinknode>");
  opt->addFlag("t");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "dot file", "<dot_filename>");
  opt->addFlag("o");
  optMgr_.regOpt(opt);
}

WriteMaxFlow::~WriteMaxFlow(){}

bool WriteMaxFlow::exec(int argc, char **argv)
{
  optMgr_.parse(argc, argv);
  if ( optMgr_.getParsedOpt("h"))
  {
    optMgr_.usage();
    return true;
  }
  char *fname, *source_name, *sink_name;
  if (optMgr_.getParsedOpt("s")) source_name = optMgr_.getParsedValue("s");
  else
  {
    fprintf(stderr, 
        "**ERROR WriteMaxFlow::exec(): source need to be specisfied\n");
    return false;
  }

  if (optMgr_.getParsedOpt("t")) sink_name = optMgr_.getParsedValue("t");
  else
  {
    fprintf(stderr, 
        "**ERROR WriteMaxFlow::exec(): sink need to be specisfied\n");
    return false;
  }

  if (optMgr_.getParsedOpt("o")) fname = optMgr_.getParsedValue("o");
  else
  {
    fprintf(stderr, 
        "**ERROR WriteMaxFlow::exec(): output dot file path is needed\n");
    return false;
  }

  if (my_graph == 0 ) 
  {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): read dot file is needed first\n");
    return false;
  }
  //TODO
   
#ifdef _TIME_ON_ 
  timeval tvS, tvE;
  CommonNs::TmUsage tmusg;
  CommonNs::TmStat stat;
  tmusg.periodStart();
  gettimeofday( &tvS, NULL);
#endif 

  ofstream outFile;
  vector< pair<unsigned,unsigned> > result;
  vector< int > value;
  string source = source_name; 
  string sink = sink_name;
  source = source.substr(1);
  sink = sink.substr(1);
  outFile.open(fname);
  unsigned verticesNum;
  unsigned maxFlow = 0;

  my_graph->printGraph();
  verticesNum = my_graph->MaxFlow( strtol(source.c_str(),NULL,0), 
        strtol(sink.c_str(),NULL,0), result, value, maxFlow);

  if ( verticesNum ) {
#ifdef _TIME_ON_ 
    gettimeofday( &tvE, NULL);
    tmusg.getTotalUsage(stat);
    writeMessageFlow(outFile, 
        value, 
        result, 
        my_graph->name,
        verticesNum,
        maxFlow,
        (double)(1000000*(tvE.tv_sec-tvS.tv_sec)+tvE.tv_usec-tvS.tv_usec)/1000000, 
        stat.vmPeak / 1024.0
        );
#endif 
    outFile.close();
  } else {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): input node# doesn't exist! \n");
    outFile.close();
    return false;
  }
}

// -------------------------------------------------------------------------- //
// @Description: is flow
// @Provides: mouda 
// -------------------------------------------------------------------------- //

IsFlow::IsFlow(const char * const name) : Cmd(name)
{
  optMgr_.setShortDes("Check if the dot file is a spanning tree");
  optMgr_.setDes("Check if the dot file is a spanning tree of the existing \
      graph. The output is simply Yes or No output to the screen. \
      No output file is needed.");
  Opt *opt = new Opt(Opt::BOOL, "print usage", "");
  opt->addFlag("h");
  opt->addFlag("help");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "sourcenode", "<sourcenode>");
  opt->addFlag("s");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "sinknode", "<sinknode>");
  opt->addFlag("t");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "dot file", "<dot_filename>");
  opt->addFlag("i");
  optMgr_.regOpt(opt);

}

IsFlow::~IsFlow(){}

bool IsFlow::exec(int argc, char **argv)
{
  optMgr_.parse(argc, argv);
  if ( optMgr_.getParsedOpt("h"))
  {
    optMgr_.usage();
    return true;
  }
  char *iname, *source_name, *sink_name;

  if (optMgr_.getParsedOpt("s")) source_name = optMgr_.getParsedValue("s");
  else
  {
    fprintf(stderr, 
        "**ERROR WriteMaxFlow::exec(): source need to be specisfied\n");
    return false;
  }

  if (optMgr_.getParsedOpt("t")) sink_name = optMgr_.getParsedValue("t");
  else
  {
    fprintf(stderr, 
        "**ERROR WriteMaxFlow::exec(): sink need to be specisfied\n");
    return false;
  }

  if (optMgr_.getParsedOpt("i")) iname = optMgr_.getParsedValue("i");
  else
  {
    fprintf(stderr, 
        "**ERROR IsSpanningTree::exec(): output dot file path is needed\n");
    return false;
  }

  if (my_graph == 0 ) 
  {
    fprintf(stderr, 
        "**ERROR WriteTreeDfs::exec(): read dot file is needed first\n");
    return false;
  }
  //TODO
  //
  ifstream inFile;
  inFile.open(iname);

  stringstream ss;
  string s, token1, ignore ,token2, graphName, number ;
  size_t found1, found2;
  int weight = 0;  

  getline(inFile,s);
  ss.str(s);
  ss >> token1 >> graphName; 

  vector< pair<unsigned, unsigned > > graph_vect;  
  vector< int > graph_weight;
  unsigned Nvalue;

  while( getline(inFile,s) ){

    ss.str(s);
    ss >> token1; 
    if (!token1.compare("}")) break; 
    ss >> ignore >> token2; 
    token1 = token1.substr(1);
    token2 = token2.substr(1);
    graph_vect.push_back( pair<unsigned, unsigned>( 
          strtol(token1.c_str(),NULL,0), strtol(token2.c_str(),NULL,0) ) 
        ); 
    found1 = s.find_first_of('"');
    found2 = s.find_last_of('"');
    number = s.substr(found1+1, found2-found1-1);
    graph_weight.push_back(atoi(number.c_str()));

    s.clear();
  }

  graph toComp(graph_vect, graph_weight, graphName);

  if ( toComp.IsFlow() ) 
    cout << "Yes" << endl; 
  else 
    cout << "No" << endl;

  inFile.close();
  
}
