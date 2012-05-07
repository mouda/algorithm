// **************************************************************************
// File       [ sys_cmd.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ some system and misc commands ]
// Date       [ Ver. 2.0 started 2010/07/01 ]
// **************************************************************************

#include <cstdlib>
#include <fstream>

#include "sys_cmd.h"

using namespace std;
using namespace CommonNs;

//{{{ class SysBashCmd   method
SysBashCmd::SysBashCmd(const char *const name) : Cmd(name) {
    optMgr_.setShortDes("opens a new bash shell environment");
    optMgr_.setDes("opens a new bash shell environment");
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}

SysBashCmd::~SysBashCmd() {}

bool SysBashCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    int res = system("bash --login");
    if (res != 0)
        return false;
    return true;
}
//}}}
//{{{ class SysListCmd   method
SysListCmd::SysListCmd(const char *const name) : Cmd(name) {
    optMgr_.setShortDes("list diectory contents");
    optMgr_.setDes("lists contents in DIRECTORY. If not specified, list current directory content.");
    optMgr_.regArg(new Arg(Arg::OPT, "target directories", "DIRECTORY"));
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}

SysListCmd::~SysListCmd() {}

bool SysListCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    char cmd[256];
    cmd[0] = '\0';
    for (int i = 0; i < argc; ++i) {
        strcat(cmd, argv[i]);
        strcat(cmd, " ");
    }
    strcat(cmd, " --color=always -F 2> /dev/null");
    if (system(cmd) != 0) {
        fprintf(stderr, "**ERROR SysListCmd::exec(): list failed\n");
        return false;
    }
    return true;
}
//}}}
//{{{ class SysCatCmd    method
SysCatCmd::SysCatCmd(const char *const name) : Cmd(name) {
    optMgr_.setShortDes("concatenate files and print on the standard output");
    optMgr_.setDes("Concatenate FILE(s), or standard input, to standard output");
    optMgr_.regArg(new Arg(Arg::REQ_INF, "files to be printed", "FILE"));
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}

SysCatCmd::~SysCatCmd() {}

bool SysCatCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (argc < 2) {
        fprintf(stderr, "**ERROR SysCatCmd::exec(): file needed\n");
        return false;
    }

    char cmd[256];
    cmd[0] = '\0';
    for (int i = 0; i < argc; ++i) {
        strcat(cmd, argv[i]);
        strcat(cmd, " ");
    }
    strcat(cmd, " 2> /dev/null");
    if (system(cmd) != 0) {
        fprintf(stderr, "**ERROR SysCatCmd::exec(): cat files failed\n");
        return false;
    }
    return true;
}
//}}}
//{{{class SysCdCmd     method
SysCdCmd::SysCdCmd(const char *const name) : Cmd(name) {
    optMgr_.setShortDes("change directory");
    optMgr_.setDes("changes working directory to DIRECTORY. If not specified, changes to home directory.");
    optMgr_.regArg(new Arg(Arg::OPT, "target directories", "DIRECTORY"));
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}
SysCdCmd::~SysCdCmd() {}

bool SysCdCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (argc == 1) {
        char *home = getenv("HOME");
        if (chdir(home) != 0) {
            fprintf(stderr, "**ERROR SysCdCmd::exec(): ");
            fprintf(stderr, "cannot change to target directory\n");
            return false;
        }
        return true;
    }
    if (argc > 1) {
        if (chdir(argv[1]) != 0) {
            fprintf(stderr, "**ERROR SysCdCmd::exec(): ");
            fprintf(stderr, "cannot change to target directory\n");
            return false;
        }
    }
    return true;
}
//}}}
//{{{ class SysPwdCmd    method
SysPwdCmd::SysPwdCmd(const char *const name) : Cmd(name) {
    optMgr_.setShortDes("print name of current directory");
    optMgr_.setDes("prints the full filename of the current working directory");
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}

SysPwdCmd::~SysPwdCmd() {}

bool SysPwdCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    int res = system("pwd");
    if (res != 0)
        return false;
    return true;
}
//}}}
//{{{ class SysSetCmd    method
SysSetCmd::SysSetCmd(const char *const name, CmdMgr *cmdMgr) : Cmd(name) {
    cmdMgr_ = cmdMgr;
    optMgr_.setShortDes("set variables");
    optMgr_.setDes("set VAR to VALUE");
    optMgr_.regArg(new Arg(Arg::OPT, "variable name", "VAR"));
    optMgr_.regArg(new Arg(Arg::OPT, "value of the variable", "VALUE"));
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}

SysSetCmd::~SysSetCmd() {}

bool SysSetCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (argc == 1) {
        vector<const char *> vars;
        cmdMgr_->getVars(vars);
        printf("%s set variables\n", cmdMgr_->getComment());
        for (size_t i = 0; i < vars.size(); ++i) {
            printf("%s    %s = %s\n", cmdMgr_->getComment(), vars[i],
                                      cmdMgr_->getValue(vars[i]));
        }
        return true;
    }

    if (argc < 3) {
        fprintf(stderr, "**ERROR SysSetCmd::exec(): ");
        fprintf(stderr, "variable and value needed\n");
        return false;
    }

    if (!cmdMgr_->addVar(argv[1], argv[2])) {
        fprintf(stderr, "**ERROR SysSetCmd::exec(): set failed\n");
        return false;
    }

    return true;
}
//}}}
//{{{ class SysExitCmd   method
SysExitCmd::SysExitCmd(const char *const name, CmdMgr *cmdMgr) : Cmd(name) {
    cmdMgr_ = cmdMgr;
    optMgr_.setShortDes("exit the program");
    optMgr_.setDes("exits the program");
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}
SysExitCmd::~SysExitCmd() {}

bool SysExitCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }
    cmdMgr_->setExit(true);
    return true;
}
//}}}
//{{{ class SysSourceCmd method
SysSourceCmd::SysSourceCmd(const char *const name, CmdMgr *cmdMgr) : Cmd(name) {
    cmdMgr_ = cmdMgr;
    optMgr_.setShortDes("run commands from startup file");
    optMgr_.setDes("runs commands from FILE");
    optMgr_.regArg(new Arg(Arg::REQ, "target file with commands", "FILE"));
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}
SysSourceCmd::~SysSourceCmd() {}

bool SysSourceCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (argc < 2) {
        fprintf(stderr, "**ERROR SysSourceCmd::exec(): ");
        fprintf(stderr, "please specify source file\n");
        return false;
    }

    FILE *fin = fopen(argv[1], "r");
    if (!fin) {
        fprintf(stderr, "**ERROR SysSourceCmd::exec(): ");
        fprintf(stderr, "file cannot be opened\n");
        return false;
    }

    char expr[4096];
    char *pos = strrchr(argv[1], '/') ? strrchr(argv[1], '/') : argv[1];
    int count  = 0;
    CmdMgr::Result res = CmdMgr::NOP;
    while (fgets(expr, 4096, fin) && res != CmdMgr::EXIT) {
        count++;
        printf("%s %d> %s\n", pos, count, expr);
        res = cmdMgr_->exec(expr);
        if (res == CmdMgr::NOT_EXIST)
            fprintf(stderr, "**ERROR command does not exist\n");
    }
    fclose(fin);

    return true;
}
//}}}
//{{{ class SysHelpCmd   method
SysHelpCmd::SysHelpCmd(const char *const name, CmdMgr *cmdMgr) : Cmd(name) {
    cmdMgr_ = cmdMgr;
    optMgr_.setShortDes("print help messages");
    optMgr_.setDes("prints help for COMMAND. If not specified, prints the usage of the command manager.");
    optMgr_.regArg(new Arg(Arg::OPT, "target command", "COMMAND"));
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}
SysHelpCmd::~SysHelpCmd() {}

bool SysHelpCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (argc == 1) {
        cmdMgr_->usage();
        return true;
    }
    if (argc > 1) {
        Cmd *cmd = cmdMgr_->getCmd(argv[1]);
        if (!cmd) {
            fprintf(stderr, "**ERROR SysHelpCmd::exec(): ");
            fprintf(stderr, "command does not exist\n");
            return false;
        }
        else
            cmd->optMgr_.usage();
    }
    return true;
}
//}}}
//{{{ class SysDotCmd    method
SysDotCmd::SysDotCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("call system dot filter");
    optMgr_.setDes("convert a graph in dot language to a picture");

    optMgr_.regArg(new Arg(Arg::REQ, "input dot file", "INPUT"));

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output format. Default is jpg",
                  "bmp|jpg|ps");
    opt->addFlag("T");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <input>.<format>",
                  "OUTPUT");
    opt->addFlag("o");
    optMgr_.regOpt(opt);
}

SysDotCmd::~SysDotCmd() {}

bool SysDotCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (optMgr_.getNParsedArg() < 1) {
        fprintf(stderr, "**ERROR SysDotCmd::exec(): need input file\n");
        return false;
    }

    string format = "jpg";
    if (optMgr_.getParsedOpt("T"))
        format = optMgr_.getParsedValue("T");

    string fname = string(optMgr_.getParsedArg(0)) + "." + format;
    if (optMgr_.getParsedOpt("o"))
        fname = optMgr_.getParsedValue("o");

    int res = 0;
    string cmd = "dot ";
    cmd += "-T" + format + " ";
    cmd += "-o" + fname + " ";
    cmd += optMgr_.getParsedArg(0);
    res = system(cmd.c_str());
    if (res != 0) {
        fprintf(stderr, "**ERROR SysDotCmd::exec(): display failed\n");
        return false;
    }
    return true;
}
//}}}


