// **************************************************************************
// File       [ sys_cmd.h ]
// Author     [ littleshamoo ]
// Synopsis   [ some system and misc commands ]
// Date       [ Ver. 2.0 started 2010/04/09 ]
// **************************************************************************

#ifndef _COMMON_SYS_CMD_H_
#define _COMMON_SYS_CMD_H_

#include "cmd.h"

namespace CommonNs {

class SysBashCmd : public Cmd {
public:
         SysBashCmd(const char *const name);
         ~SysBashCmd();

    bool exec(int argc, char **argv);
};

class SysListCmd : public Cmd {
public:
         SysListCmd(const char *const name);
         ~SysListCmd();

    bool exec(int argc, char **argv);
};

class SysCatCmd : public Cmd {
public:
         SysCatCmd(const char *const name);
         ~SysCatCmd();

    bool exec(int argc, char **argv);
};

class SysCdCmd : public Cmd {
public:
         SysCdCmd(const char *const name);
         ~SysCdCmd();

    bool exec(int argc, char **argv);
};

class SysPwdCmd : public Cmd {
public:
         SysPwdCmd(const char *const name);
         ~SysPwdCmd();

    bool exec(int argc, char **argv);
};

class SysSetCmd : public Cmd {
public:
         SysSetCmd(const char *const name, CmdMgr *mgr);
         ~SysSetCmd();

    bool exec(int argc, char **argv);

private:
    CmdMgr *cmdMgr_;
};

class SysExitCmd : public Cmd {
public:
         SysExitCmd(const char *const name, CmdMgr *mgr);
         ~SysExitCmd();

    bool exec(int argc, char **argv);

private:
    CmdMgr *cmdMgr_;
};

class SysSourceCmd : public Cmd {
public:
         SysSourceCmd(const char *const name, CmdMgr *mgr);
         ~SysSourceCmd();

    bool exec(int argc, char **argv);

private:
    CmdMgr *cmdMgr_;
};

class SysHelpCmd : public Cmd {
public:
         SysHelpCmd(const char *const name, CmdMgr *mgr);
         ~SysHelpCmd();

    bool exec(int argc, char **argv);

private:
    CmdMgr *cmdMgr_;
};

class SysDotCmd : public CommonNs::Cmd {
public:
         SysDotCmd(const char *const name);
         ~SysDotCmd();

    bool exec(int argc, char **argv);
};



class SysDisplayCmd : public Cmd {
public:

         SysDisplayCmd(const char *const name);
         ~SysDisplayCmd();

     bool exec(int argc, char **argv);
};


};  // namespace

#endif

