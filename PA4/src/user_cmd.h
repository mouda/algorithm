// **************************************************************************
// File       [ test_cmd.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#ifndef _TEST_CMD_H_
#define _TEST_CMD_H_

#include "../lib/cmd.h"

class TestCmd : public CommonNs::Cmd {
public:
         TestCmd(const char * const name);
         ~TestCmd();

    bool exec(int argc, char **argv);
};

class ReadGraph : public CommonNs::Cmd {
public:
         ReadGraph(const char * const name);
         ~ReadGraph();

    bool exec(int argc, char **argv);
};


class WriteTreeDfs : public CommonNs::Cmd {
public:
         WriteTreeDfs(const char * const name);
         ~WriteTreeDfs();

    bool exec(int argc, char **argv);
};

class WriteTreeBfs : public CommonNs::Cmd {
public:
         WriteTreeBfs(const char * const name);
         ~WriteTreeBfs();

    bool exec(int argc, char **argv);
};

class WriteTreeMst : public CommonNs::Cmd {
public:
         WriteTreeMst(const char * const name);
         ~WriteTreeMst();

    bool exec(int argc, char **argv);
};

class IsSpanningTree : public CommonNs::Cmd {
public:
         IsSpanningTree(const char * const name);
         ~IsSpanningTree();

    bool exec(int argc, char **argv);
};

class WriteMaxFlow : public CommonNs::Cmd {
public:
         WriteMaxFlow(const char * const name);
         ~WriteMaxFlow();

    bool exec(int argc, char **argv);
};


class IsFlow : public CommonNs::Cmd {
public:
         IsFlow(const char * const name);
         ~IsFlow();

    bool exec(int argc, char **argv);
};
#endif

