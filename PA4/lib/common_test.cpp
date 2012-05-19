// **************************************************************************
// File       [ common_test.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ Example of the utility package ]
// Date       [ 2010/12/10 created ]
// **************************************************************************

#include <cstdlib>
#include <sys/times.h>

using namespace std;

#include "tm_usage.h"
#include "get_opt.h"
#include "cmd.h"
#include "sys_cmd.h"

using namespace CommonNs;

void initOpts(OptMgr *mgr);
void initCmds(CmdMgr *mgr);
void usage(OptMgr *mgr);

int main(int argc, char **argv) {
    TmUsage tmUsg;
    tmUsg.totalStart();

    OptMgr *optMgr = new OptMgr;
    CmdMgr *cmdMgr = new CmdMgr;

    initOpts(optMgr);
    initCmds(cmdMgr);

    optMgr->parse(argc, argv);
    if (optMgr->getParsedOpt("h")) {
        optMgr->usage();
        exit(0);
    }

    // interactive shell
    printf("** This is the common package test binary\n");
    printf("** Please report any bugs to littleshamoo. Thanks\n");
    CmdMgr::Result res = CmdMgr::SUCCESS;
    cmdMgr->setComment("#");
    cmdMgr->setPrompt("test> ");
    cmdMgr->setColor(CmdMgr::YELLOW);
    while (res != CmdMgr::EXIT) {
        res = cmdMgr->read();
        if (res == CmdMgr::NOT_EXIST) {
            fprintf(stderr, "**ERROR main(): command ");
            fprintf(stderr, "`%s' not found\n", cmdMgr->getErrorStr());
            continue;
        }
        if (res != CmdMgr::NOP)
            printf("%d\n", res);
    }

    TmStat tmEnd;
    if (!tmUsg.getTotalUsage(tmEnd))
        fprintf(stderr, "**ERROR main(): cannot get resource usage\n");
    printf("# Runtime and memory usage\n");
    printf("# user time: %.2fs\n", (double)tmEnd.uTime / 1000000.0);
    printf("# sys time:  %.2fs\n", (double)tmEnd.sTime / 1000000.0);
    printf("# real time: %.2fs\n", (double)tmEnd.rTime / 1000000.0);
    printf("# cur mem:   %.2fMB\n", (double)tmEnd.vmSize / 1024.0);
    printf("# peak mem:  %2fMB\n", (double)tmEnd.vmPeak / 1024.0);
    printf("# diff mem:  %2fMB\n", (double)tmEnd.vmDiff / 1024.0);
    printf("\n");
    printf("** Goodbye\n");

    return 0;
}


void usage(OptMgr *mgr) {
    mgr->usage();
}

class TestLargeLoopCmd : public Cmd {
public:
         TestLargeLoopCmd(const char *const name);
         ~TestLargeLoopCmd();

    bool exec(int argc, char **argv);
};

TestLargeLoopCmd::TestLargeLoopCmd(const char *const name) : Cmd(name) {
    optMgr_.setName("test_large_loop");
    optMgr_.setShortDes("evaluate a large loop to test runtime");
    string des = "test_large_loop constructs a three-level loop with size";
    des += " based on the integer argument and performs arithmetic";
    des += " operations.";
    optMgr_.setDes(des.c_str());

    des = "Loop size. Default is 1500 if not set.";
    optMgr_.regArg(new Arg(Arg::OPT, des.c_str(), "size"));

    Opt *opt;
    des = "Test mode selection ('a' for stuck-at fault, 's' for";
    des += " launch-off-shift transition fault and 'c' for";
    des += " launch-off-capture.)";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "<a|s|c>");
    opt->addFlag("m");
    opt->addFlag("mode");
    optMgr_.regOpt(opt);

}

TestLargeLoopCmd::~TestLargeLoopCmd() {}

bool TestLargeLoopCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    int size = optMgr_.getNParsedArg() > 0 ?
                 atoi(optMgr_.getParsedArg(0)) : 1500;

    int res = 0;
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            for (int k = 0; k < size; ++k)
                res += i + j * k;

    printf("result: %d\n", res);

    return true;
}

//{{{ void initOpts(OptMgr *mgr)
void initOpts(OptMgr *mgr) {
    mgr->setName("fan");
    mgr->setShortDes("An ATPG tool based on FAN algorithm");

    string des = "fan takes an input verilog gate-level netlist and";
    des += " performs test generation or fault simulation. Currently";
    des += " supports stuck-at fault model and transition delay fault";
    des += " model. Additional functions include test relaxation and";
    des += " SAT-based test generation.";
    mgr->setDes(des.c_str());

    des = "Input circuit netlist.";
    mgr->regArg(new Arg(Arg::REQ, des.c_str(), "circuit_netlist"));

    Opt *opt;
    des = "Test mode selection ('a' for stuck-at fault, 's' for";
    des += " launch-off-shift transition fault and 'c' for";
    des += " launch-off-capture.)";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "<a|s|c>");
    opt->addFlag("m");
    opt->addFlag("mode");
    mgr->regOpt(opt);


    des = "Enable random simulation and the random_limit specifies the";
    des += " iteration of simulation times.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "random_limit");
    opt->addFlag("r");
    opt->addFlag("random");
    mgr->regOpt(opt);

    des = "Limitation of dynamic compaction. It specifies the maximum";
    des += " number of failing times. Default = 300. If specified '-1' then";
    des += " dynamic compaciton will be turned off. The deterministic test";
    des += " genration will be disabled if '-2' is specified. Note that";
    des += " random simulation '-r' option should be turned on or this";
    des += " option will be ignored.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "dynamic_fail_limit");
    opt->addFlag("d");
    mgr->regOpt(opt);

    des = "Limitation of dynamic compaction. It specifies the maximum";
    des += " number of compaction fault in one pattern.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "dynamic_fault_num");
    opt->addFlag("D");
    mgr->regOpt(opt);

    des = "Backtrack limit of the FAN algorithm.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "backtrack_limit");
    opt->addFlag("b");
    mgr->regOpt(opt);

    des = "Output patterns to pattern_file.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "pattern_file");
    opt->addFlag("f");
    mgr->regOpt(opt);

    des = "Output undetected faults to undetected_file.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "undetected_file");
    opt->addFlag("u");
    mgr->regOpt(opt);

    des = "N-detect mode with n_num detection.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "n_num");
    opt->addFlag("N");
    mgr->regOpt(opt);

    des = "Enable MT-fill, an optional limit specifies the limit of fill";
    des += " ratio, range from 0.0 ~ 1.0.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "mt_fill_num");
    opt->addFlag("a");
    mgr->regOpt(opt);

    des = "Do not fill don't care bits during test generation.";
    opt = new Opt(Opt::BOOL, des.c_str(), "");
    opt->addFlag("x");
    mgr->regOpt(opt);

    des = "Fault simulation for the sim_file. Note that if '-f' option is";
    des += " applied, the fault dictionary will be dumped.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "sim_file");
    opt->addFlag("y");
    mgr->regOpt(opt);

    des = "Perform test relaxation on patterns in relax_file.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "relax_file");
    opt->addFlag("X");
    mgr->regOpt(opt);

    des = "At-speed delay test mode. Note that this only works for";
    des += " launch-off-capture transition fault test.";
    opt = new Opt(Opt::BOOL, des.c_str(), "");
    opt->addFlag("i");
    mgr->regOpt(opt);

    des = "Enable SAT-based test pattern generation.";
    opt = new Opt(Opt::BOOL, des.c_str(), "");
    opt->addFlag("s");
    mgr->regOpt(opt);

    des = "Mask PO or PPO in the mask_file. If it is not specified, all POs";
    des += " and PPOs will be masked.";
    opt = new Opt(Opt::STR_OPT, des.c_str(), "mask_file");
    opt->addFlag("M");
    mgr->regOpt(opt);

    des = "Constraint PI or PPI value in the constraint_file.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "constraint_file");
    opt->addFlag("c");
    mgr->regOpt(opt);

    des = "Extract fault list from fault_list_file.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "fault_list_file");
    opt->addFlag("l");
    mgr->regOpt(opt);

    des = "Output a verilog test bench to vlog_test_file.";
    opt = new Opt(Opt::STR_REQ, des.c_str(), "vlog_test_file");
    opt->addFlag("v");
    mgr->regOpt(opt);

    des = "Print this usage.";
    opt = new Opt(Opt::BOOL, des.c_str(), "");
    opt->addFlag("h");
    opt->addFlag("help");
    mgr->regOpt(opt);
}
//}}}
//{{{ void initCmds(CmdMgr *mgr)
void initCmds(CmdMgr *mgr) {
    // system commands
    Cmd *bashCmd   = new SysBashCmd("bash");
    Cmd *listCmd   = new SysListCmd("ls");
    Cmd *cdCmd     = new SysCdCmd("cd");
    Cmd *catCmd    = new SysCatCmd("cat");
    Cmd *pwdCmd    = new SysPwdCmd("pwd");
    Cmd *setCmd    = new SysSetCmd("set", mgr);
    Cmd *exitCmd   = new SysExitCmd("exit", mgr);
    Cmd *quitCmd   = new SysExitCmd("quit", mgr);
    Cmd *sourceCmd = new SysSourceCmd("source", mgr);
    Cmd *helpCmd   = new SysHelpCmd("help", mgr);
    mgr->regCmd(bashCmd);
    mgr->regCmd(listCmd);
    mgr->regCmd(cdCmd);
    mgr->regCmd(catCmd);
    mgr->regCmd(pwdCmd);
    mgr->regCmd(setCmd);
    mgr->regCmd(exitCmd);
    mgr->regCmd(quitCmd);
    mgr->regCmd(sourceCmd);
    mgr->regCmd(helpCmd);

    // test commands
    Cmd *test1Cmd = new TestLargeLoopCmd("test_large_loop");
    mgr->regCmd(test1Cmd);
}
//}}}

