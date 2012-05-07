// **************************************************************************
// File       [ cmd.h ]
// Author     [ littleshamoo ]
// Synopsis   [ header file for commands ]
// Date       [ Ver. 2.0 started 2010/04/09 ]
// **************************************************************************


#ifndef _COMMON_CMD_H_
#define _COMMON_CMD_H_

#include <sys/ioctl.h>  // getting window size

#include <cstdio>
#include <vector>

#include "ascii.h"
#include "get_opt.h"
#include "hash.h"

namespace CommonNs {

//{{{ class Cmd
class Cmd {
public:
                       Cmd(const char * const name);
    virtual            ~Cmd();
    virtual bool       exec(int argc, char **argv)  = 0;
            const char *getName() const;
            OptMgr     optMgr_;
}; //}}}
//{{{ class CmdMgr
class CmdMgr {
public:

    enum       Result { EXIT = -1, SUCCESS, FAIL, NOT_EXIST, NOP };
    enum       Error { E_EXIST = 0, E_NOT_REG, E_INVALID_VAR };
    enum       Color { BLACK = 0, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN,
                       WHITE };

               CmdMgr(const int &hashSize = PRIMES[0]);
               ~CmdMgr();

    // setting interface
    void       setComment(const char * const comment);
    const char *getComment() const;
    void       setPrompt(const char * const prompt);
    const char *getPrompt() const;
    void       setColor(const Color &color);
    void       setExit(const bool &exit);
    bool       getExit() const;

    // commands
    bool       regCmd(Cmd * const cmd);
    Cmd        *getCmd(const char * const name) const;

    // execution
    Result     read();
    Result     exec(const char * const input);
    Error      getError() const;
    const char *getErrorStr() const;

    // setting variables
    bool       addVar(const char * const var, char * const value);
    void       getVars(std::vector<const char *> &vars) const;
    const char *getValue(const char * const var) const;

    void       usage(FILE *out = stdout);

private:
    static const size_t NAME_LEN = 256;
    static const size_t EXPR_LEN = 4096;

    char                prompt_[NAME_LEN];   // command line prompt
    char                comment_[NAME_LEN];  // comment pattern
    char                errorStr_[NAME_LEN]; // error related string
    Color               color_;              // prompt color
    Error               error_;              // error type
    bool                exit_;               // exit flag

    StringHash<Cmd *>   *cmds_;              // command hash by name
    StringHash<char *>  *vars_;              // variable hash by name
    std::vector<char *> his_;                // command history
    size_t              hisPtr_;             // command history pointer
    char                input_[EXPR_LEN];    // user input command string
    char                inputBak_[EXPR_LEN]; // backup
    size_t              csrPos_;             // cursor position
    size_t              maxPos_;             // max input position

    // for setting variables
    char *expandVar(char *expr) const;
    char *expandHome(char *path) const;
    bool isLegalVar(const char *var) const;
    bool isLegalChar(const char &ch) const;

    // stdin and stdout setting
    void setStdin() const;
    void resetStdin() const;
    bool redirStdout(const char *const fname, const char *const mode,
                     int &fd, fpos_t &fpos);
    void resetStdout(const int &fd, const fpos_t &fpos) const;

    // user interface
    void handleNonprintable(const char &ch);
    void handleEscape();
    void handleBracket();
    void parse(std::vector<char *> &args, const char * const expr) const;
    void autoCmplt();
    void autoCmpltCmd(char *pre, std::vector<char *> &cddts,
                      const std::vector<char *> &args);
    void autoCmpltOpt(char *pre, std::vector<char *> &cddts,
                      const std::vector<char *> &args);
    void autoCmpltVar(char *pre, std::vector<char *> &cddts,
                      const std::vector<char *> &args);
    void autoCmpltFile(char *pre, std::vector<char *> &cddts,
                       const std::vector<char *> &args);
    void autoCmpltAppend(int &nappend, const char *const pre,
                         const std::vector<char *> &cddts);
    void autoCmpltShow(const int &nappend, const std::vector<char *> &cddts);
    void refresh();

    // misc
    void getDirent(std::vector<char *> &ents, const char * const path) const;
    int  getWinCol() const;
    int  strcmn(const char * const s1, const char * const s2) const;
    int  strcmn(const std::vector<char *> &strs) const;
    void freebuf(std::vector<char *> &buf);
}; //}}}

// inline methods
//{{{ class Cmd
inline Cmd::Cmd(const char * const name) {
    optMgr_.setName(name);
}

inline Cmd::~Cmd() {}

inline const char *Cmd::getName() const {
    return optMgr_.getName();
}
//}}}
//{{{ class CmdMgr
inline CmdMgr::CmdMgr(const int &hashSize) {
    strcpy(prompt_, "cmd> ");
    strcpy(comment_, "#");
    memset(input_, '\0', EXPR_LEN);
    color_ = WHITE;
    error_ = E_EXIST;
    exit_  = false;
    cmds_  = new StringHash<Cmd *>(hashSize);
    vars_  = new StringHash<char *>(hashSize);
    hisPtr_ = 0;
    csrPos_ = 0;
    maxPos_ = strlen(prompt_);
}

inline CmdMgr::~CmdMgr() {
    delete cmds_;
    delete vars_;
    for (size_t i = 0; i < his_.size(); ++i)
        delete [] his_[i];
}

inline void CmdMgr::setComment(const char * const comment) {
    strcpy(comment_, comment);
}

inline const char *CmdMgr::getComment() const {
    return comment_;
}

inline void CmdMgr::setPrompt(const char * const prompt) {
    strcpy(prompt_, prompt);
}

inline const char *CmdMgr::getPrompt() const {
    return prompt_;
}

inline void CmdMgr::setColor(const Color &color) {
    color_ = color;
}

inline void CmdMgr::setExit(const bool &exit) {
    exit_ = exit;
}

inline bool CmdMgr::getExit() const {
    return exit_;
}

inline bool CmdMgr::regCmd(Cmd * const cmd) {
    if (cmds_->count(cmd->getName()) > 0) {
        strcpy(errorStr_, "Command already registered");
        error_ = E_EXIST;
        return false;
    }
    cmds_->add(cmd->getName(), cmd);
    return true;
}

inline Cmd *CmdMgr::getCmd(const char * const name) const {
    Cmd *cmd = NULL;
    cmds_->lookup(name, cmd);
    return cmd;
}

inline bool CmdMgr::addVar(const char * const var, char * const value) {
    if (!isLegalVar(var)) {
        strcpy(errorStr_, var);
        error_ = E_INVALID_VAR;
        return false;
    }
    char expand[EXPR_LEN];
    strcpy(expand, value);
    expandHome(expand);
    expandVar(expand);
    char *expr = strdup(expand);
    vars_->add(var, expr);
    return true;
}

inline void CmdMgr::getVars(std::vector<const char *> &vars) const {
    vars.clear();
    StringHashNode<char *> **bank = vars_->head();
    for ( ; bank != vars_->head() + vars_->nbank(); bank++) {
        StringHashNode<char *> *cur = (*bank);
        while (cur) {
            vars.push_back(cur->key_);
            cur = cur->next_;
        }
    }
}

inline const char *CmdMgr::getValue(const char * const var) const {
    char *value = NULL;
    vars_->lookup(var, value);
    return value;
}

inline bool CmdMgr::isLegalVar(const char * const var) const {
    for (size_t i = 0; i < strlen(var); ++i)
        if (!isLegalChar(var[i]))
            return false;
    return true;
}

inline bool CmdMgr::isLegalChar(const char &ch) const {
    if (ch >= '0' && ch <= '9')
        return true;
    if (ch >= 'A' && ch <= 'Z')
        return true;
    if (ch >= 'a' && ch <= 'z')
        return true;
    if (ch == '_')
        return true;
    return false;
}

inline CmdMgr::Error CmdMgr::getError() const {
    return error_;
}

inline const char *CmdMgr::getErrorStr() const {
    return errorStr_;
}

inline int CmdMgr::getWinCol() const {
    winsize ts;
    ioctl(fileno(stdout), TIOCGWINSZ, &ts);
    return ts.ws_col;
}

inline int CmdMgr::strcmn(const char * const s1, const char * const s2)
  const {
    unsigned int i = 0;
    for ( ; i < strlen(s1) && i < strlen(s2) && s1[i] == s2[i]; ++i)
        ;
    return i;
}

inline int CmdMgr::strcmn(const std::vector<char *> &strs) const {
    if (strs.size() == 0)
        return 0;
    int len = strlen(strs[0]);
    for (size_t i = 1; i < strs.size(); ++i) {
        int cmn = strcmn(strs[i - 1], strs[i]);
        if (cmn < len)
            len = cmn;
    }
    return len;
}

inline void CmdMgr::freebuf(std::vector<char *> &buf) {
    for (size_t i = 0; i < buf.size(); ++i)
        delete [] buf[i];
    buf.clear();
}
//}}}

};


#endif


