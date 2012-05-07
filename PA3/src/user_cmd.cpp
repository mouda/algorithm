// **************************************************************************
// File       [ test_cmd.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#include "user_cmd.h"

using namespace std;
using namespace CommonNs;

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

  opt = new Opt(Opt::STR_REQ, "source node", "<sourcenode>");
  opt->addFlag("s");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "dot file", "<dot_filename>");
  opt->addFlag("o");
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
  char *sname, *fname;
  if (optMgr_.getParsedOpt("s"))
    sname = optMgr_.getParsedValue("s");
  else
  {
    fprintf(stderr, "**ERROR ReadGraph::exec(): input node # is needed\n");
    return false;
  }

  if (optMgr_.getParsedOpt("o")) fname = optMgr_.getParsedValue("o");
  else
  {
    fprintf(stderr, 
        "**ERROR ReadGraph::exec(): output dot file path is needed\n");
    return false;
  }

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

  //TODO
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

  opt = new Opt(Opt::STR_REQ, "source node", "<sourcenode>");
  opt->addFlag("s");
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

  opt = new Opt(Opt::STR_REQ, "source node", "<sourcenode>");
  opt->addFlag("s");
  optMgr_.regOpt(opt);

  opt = new Opt(Opt::STR_REQ, "dot file", "<dot_filename>");
  opt->addFlag("o");
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
