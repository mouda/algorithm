// **************************************************************************
// File       [ cmd.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ methods for command manager ]
// Date       [ Ver. 2.0 started 2010/04/09 ]
// **************************************************************************

#include <sys/stat.h>   // directory testing
#include <sys/types.h>  // file redirection
#include <pwd.h>        // user home path
#include <unistd.h>     // current user ID
#include <termios.h>    // setting stdin
#include <dirent.h>     // getting directory entries
#include <cstdlib>      // getting environment variable
#include <cstring>
#include <iomanip>

#include "ascii.h"
#include "vt100.h"
#include "cmd.h"

using namespace std;
using namespace CommonNs;

//{{{ char *CmdMgr::expandVar(char *) const
char *CmdMgr::expandVar(char *expr) const {
    char expand[EXPR_LEN];
    char var[NAME_LEN];
    int j = 0;
    for (unsigned int i = 0; i < strlen(expr); ++i) {
        if (expr[i] == '$') {
            memset(var, '\0', NAME_LEN);
            i++;
            if (i < strlen(expr) && expr[i] == '{')
                i++;
            int begin = i;
            while (i < strlen(expr) && isLegalChar(expr[i])) {
                var[i - begin] = expr[i];
                i++;
            }
            char *value = NULL;
            vars_->lookup(var, value);
            if (!value)
                value = getenv(var);
            for (unsigned int k = 0; value && k < strlen(value); ++j, ++k)
                expand[j] = value[k];
            if (i < strlen(expr) && expr[i] != '}')
                i--;
        }
        else {
            expand[j] = expr[i];
            j++;
        }
    }
    expand[j] = '\0';
    return strcpy(expr, expand);
} //}}}
//{{{ char *CmdMgr::expandHome(char *) const
char *CmdMgr::expandHome(char *path) const {
    char expand[EXPR_LEN];
    char uname[NAME_LEN];
    int j = 0;
    for (unsigned int i = 0; i < strlen(path); ++i) {
        if (path[i] == '~') {
            memset(uname, '\0', NAME_LEN);
            i++;
            int begin = i;
            while (i < strlen(path) && isLegalChar(path[i])) {
                uname[i - begin] = path[i];
                i++;
            }
            struct passwd *profile = NULL;
            if (strlen(uname) == 0)
                profile = getpwuid(getuid());
            else
                profile = getpwnam(uname);
            for (unsigned int k = 0;  profile && k < strlen(profile->pw_dir);
                 ++j, ++k)
                expand[j] = profile->pw_dir[k];
            i--;
        }
        else {
            expand[j] = path[i];
            j++;
        }
    }
    expand[j] = '\0';
    return strcpy(path, expand);
} //}}}
//{{{ void CmdMgr::getDirent(vector<char *> &, const char *const) const
void CmdMgr::getDirent(vector<char *> &ents, const char *const path) const {
    ents.clear();
    DIR *dir;
    dirent *ent;
    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0
                && strcmp(ent->d_name, "..") != 0)
                ents.push_back(strcpy(new char[strlen(ent->d_name) + 1],
                                      ent->d_name));
        }
        closedir(dir);
    }
} //}}}

//{{{ void CmdMgr::setStdin() const
void CmdMgr::setStdin() const {
    int fd = fileno(stdin);
    termios tcflags;
    tcgetattr(fd, &tcflags);
    tcflags.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(fd, TCSANOW, &tcflags);
} //}}}
//{{{ void CmdMgr::resetStdin() const
void CmdMgr::resetStdin() const {
    int fd = fileno(stdin);
    termios tcflags;
    tcgetattr(fd, &tcflags);
    tcflags.c_lflag |= ICANON | ECHO;
    tcsetattr(fd, TCSANOW, &tcflags);
} //}}}
//{{{ bool CmdMgr::redirStdout() const
bool CmdMgr::redirStdout(const char *const fname, const char *const mode,
                         int &fd, fpos_t &fpos) {
    fd = dup(fileno(stdout));
    fgetpos(stdout, &fpos);
    FILE *fptr = fopen(fname, mode);
    if (fptr) {
        fclose(fptr);
        if (!freopen(fname, mode, stdout)) {
            strcpy(errorStr_, "file cannot be written");
            return false;
        }
    }
    else {
        strcpy(errorStr_, "file cannot be written");
        return false;
    }
    return true;
} //}}}
//{{{ void CmdMgr::resetStdout() const
void CmdMgr::resetStdout(const int &fd, const fpos_t &fpos) const {
        fflush(stdout);
        dup2(fd, fileno(stdout));
        close(fd);
        clearerr(stdout);
        fsetpos(stdout, &fpos);
} //}}}

//{{{ void CmdMgr::handleNonprintable(const char &ch)
void CmdMgr::handleNonprintable(const char &ch) {
    switch(ch) {
        case ASCII_DEL:
        case ASCII_BS:
            if (csrPos_ > 0) {
                memmove(input_ + csrPos_ - 1, input_ + csrPos_,
                        strlen(&input_[csrPos_]) + 1);
                csrPos_--;
                refresh();
            }
            break;
        case ASCII_HT:
            autoCmplt();
            refresh();
            break;
        case ASCII_ESC:
            handleEscape();
            break;
        default:
          break;
    }
} //}}}
//{{{ void CmdMgr::handleEscape()
void CmdMgr::handleEscape() {
    int ch1, ch2;
    switch (ch1 = getchar()) {
        case ASCII_L_SQU:
            handleBracket();
            break;
        case ASCII_O:
            // HOME: "\033OH"
            // END:  "\033OF"
            if ((ch2 = getchar()) == ASCII_H)
                csrPos_ = 0;
            else if (ch2 == ASCII_F)
                csrPos_ = strlen(input_);
            refresh();
            break;
        default:
            break;
    }
} //}}}
//{{{ void CmdMgr::handleBracket()
void CmdMgr::handleBracket() {
    int ch;
    switch (ch = getchar()) {
        case ASCII_A: // ARROW_UP: "\033[A"
            if (hisPtr_ == his_.size())
                strcpy(inputBak_, input_);
            if (hisPtr_ > 0) {
                hisPtr_--;
                strcpy(input_, his_[hisPtr_]);
                csrPos_ = strlen(input_);
            }
            refresh();
            break;
        case ASCII_B: // ARROW_DOWN: "\033[B"
            if (hisPtr_  + 1 < his_.size()) {
                hisPtr_++;
                strcpy(input_, his_[hisPtr_]);
                csrPos_ = strlen(input_);
            }
            else if (hisPtr_ + 1 == his_.size()) {
                hisPtr_ = his_.size();
                strcpy(input_, inputBak_);
                csrPos_ = strlen(input_);
            }
            refresh();
            break;
        case ASCII_C: // ARROW_RIGHT: "\033[C"
            if (csrPos_ < strlen(input_)) {
                csrPos_++;
                refresh();
            }
            break;
        case ASCII_D: // ARROW_LEFT: "\033[D"
            if (csrPos_ > 0) {
                csrPos_--;
                refresh();
            }
            break;
        case ASCII_1: // HOME: "\033[1"
            getchar(); // eat "~"
            csrPos_ = 0;
            refresh();
            break;
        case ASCII_3: // DELETE: "\033[3"
            getchar(); // eat "~"
            if (csrPos_ < strlen(input_)) {
                memmove(input_ + csrPos_, input_ + csrPos_ + 1,
                        strlen(&input_[csrPos_]) + 1);
                refresh();
            }
            break;
        case ASCII_4: // END: "\033[4"
            getchar(); // eat "~"
            csrPos_ = strlen(input_);
            refresh();
            break;
        case ASCII_2: // INSERT: "\033[2"
        case ASCII_5: // PGUP:   "\033[5"
        case ASCII_6: // PGDN:   "\033[6"
            getchar(); // eat "~"
            break;
        default:
            break;
      }
} //}}}
//{{{ void CmdMgr::refresh()
void CmdMgr::refresh() {
    // scroll screen on boundary condition
    if (strlen(prompt_) + strlen(input_) > maxPos_) {
        int nlinePrev = maxPos_ / getWinCol();
        maxPos_ = strlen(prompt_) + strlen(input_);
        int nlineCur  = maxPos_ / getWinCol();
        for (int i = 0; i < nlineCur - nlinePrev; ++i)
            printf("%s", VT100_SCRU);
    }

    // clear current text
    int nrow = maxPos_ / getWinCol();
    printf("%s", VT100_CSRR);
    for (int i = 0; i < nrow; ++i)
        printf("%s", VT100_CSRU);
    printf("%s", VT100_ERSD);
    fflush(stdout);

    // reprint prompt and cmd string
    switch (color_) {
        case BLACK:
            printf("%s", ANSI_BLACK);
            break;
        case RED:
            printf("%s", ANSI_RED);
            break;
        case GREEN:
            printf("%s", ANSI_GREEN);
            break;
        case YELLOW:
            printf("%s", ANSI_YELLOW);
            break;
        case BLUE:
            printf("%s", ANSI_BLUE);
            break;
        case PURPLE:
            printf("%s", ANSI_PURPLE);
            break;
        case CYAN:
            printf("%s", ANSI_CYAN);
            break;
        case WHITE:
            printf("%s", ANSI_WHITE);
            break;
        default:
            printf("%s", ANSI_WHITE);
            break;
    }
    printf("%s%s%s", prompt_, ANSI_RESET, input_);

    // move cursor back to cursor position
    int csrPos = strlen(prompt_) + csrPos_;
    int row = nrow - csrPos / getWinCol();
    int col = csrPos % getWinCol();
    printf("%s", VT100_CSRR);
    for (int i = 0; i < row; ++i)
        printf("%s", VT100_CSRU);
    for (int i = 0; i < col; ++i)
        printf("%s", VT100_CSRF);

    fflush(stdout);
}
//}}}

//{{{ void CmdMgr::autoCmplt()
void CmdMgr::autoCmplt() {
    char expr[NAME_LEN];
    strncpy(expr, input_, csrPos_);
    expr[csrPos_] = '\0';
    vector<char *> args;
    parse(args, expr);

    bool iscmd = args.size() == 0
                 || (args.size() == 1 && input_[csrPos_ - 1] != ' ');
    bool isopt = args.size() > 1 && input_[csrPos_ - 1] != ' '
                 && args[args.size() - 1][0] == '-'
                 && cmds_->count(args[0]);
    bool isvar = args.size() > 1 && input_[csrPos_ - 1] != ' '
                 && args[args.size() - 1][0] == '$'
                 && !strchr(args[args.size() - 1], '}')
                 && !strchr(args[args.size() - 1], '/');

    vector<char *> cddts;
    char pre[NAME_LEN];
    memset(pre, '\0', NAME_LEN);
    if (iscmd)
        autoCmpltCmd(pre, cddts, args);
    else if (isopt)
        autoCmpltOpt(pre, cddts, args);
    else if (isvar)
        autoCmpltVar(pre, cddts, args);
    else // files or directories
        autoCmpltFile(pre, cddts, args);

    // append common prefix
    int nappend = 0;
    autoCmpltAppend(nappend, pre, cddts);
    autoCmpltShow(nappend, cddts);

    // free
    freebuf(args);
    freebuf(cddts);
}
//}}}
//{{{ void CmdMgr::autoCmpltCmd()
void CmdMgr::autoCmpltCmd(char *pre, vector<char *> &cddts,
                          const vector<char *> &args) {
    if (args.size() == 0)
        strcpy(pre, "");
    else
        strcpy(pre, args[0]);

    cddts.clear();
    StringHashNode<Cmd *> **bank = cmds_->head();
    for ( ; bank != cmds_->head() + cmds_->nbank(); bank++) {
        StringHashNode<Cmd *> *cur = (*bank);
        while (cur) {
            if (strncmp(pre, cur->key_, strlen(pre)) == 0)
                cddts.push_back(strdup(cur->key_));
            cur = cur->next_;
        }
    }
} //}}}
//{{{ void CmdMgr::autoCmpltOpt()
void CmdMgr::autoCmpltOpt(char *pre, vector<char *> &cddts,
                          const vector<char *> &args) {
    Cmd *cmd = NULL;
    cmds_->lookup(args[0], cmd);

    bool longOpt = strlen(args[args.size() - 1]) > 1
                   && args[args.size() - 1][1] == '-';
    if (longOpt)
        strcpy(pre, args[args.size() - 1] + 2);
    else
        strcpy(pre, args[args.size() - 1] + 1);

    cddts.clear();
    for (size_t i = 0; i < cmd->optMgr_.getNFlag(); ++i) {
        char *flag = cmd->optMgr_.getFlag(i);
        if (strncmp(pre, flag, strlen(pre)) == 0) {
            if ((strlen(flag) == 1 && !longOpt)
                || (strlen(flag) > 1 && longOpt))
                cddts.push_back(strcpy(new char[strlen(flag) + 1], flag));
        }
    }
} //}}}
//{{{ void CmdMgr::autoCmpltVar()
void CmdMgr::autoCmpltVar(char *pre, vector<char *> &cddts,
                          const vector<char *> &args) {
    bool longVar = strlen(args[args.size() - 1]) > 1
                   && args[args.size() - 1][1] == '{';
    if (longVar)
        strcpy(pre, args[args.size() - 1] + 2);
    else
        strcpy(pre, args[args.size() - 1] + 1);

    cddts.clear();
    StringHashNode<char *> **bank = vars_->head();
    for ( ; bank != vars_->head() + vars_->nbank(); bank++) {
        StringHashNode<char *> *cur = (*bank);
        while (cur) {
            if (strncmp(pre, cur->key_, strlen(pre)) == 0)
                cddts.push_back(strcpy(new char[strlen(cur->key_) + 1],
                                       cur->key_));
            cur = cur->next_;
        }
    }
} //}}}
//{{{ void CmdMgr::autoCmpltFile()
void CmdMgr::autoCmpltFile(char *pre, vector<char *> &cddts,
                           const vector<char *> &args) {
    // find directory string and prefix
    char dir[EXPR_LEN];
    char *path;
    if (input_[csrPos_ - 1] != ' ') {
        path = args[args.size() - 1];
        char *ch;
        if (!(ch = strrchr(path, '/'))) {
            strcpy(dir, "./");
            strcpy(pre, path);
        }
        else {
            strncpy(dir, path, ch - path);
            dir[ch - path] = '\0';
            expandVar(dir);
            expandHome(dir);
            strcat(dir, "/");
            strcpy(pre, ch + 1);
        }
    }
    else {
        strcpy(dir, "./");
        strcpy(pre, "");
    }

    // find out content candidates and their common prefix
    vector<char *> ents;
    getDirent(ents, dir);
    char file[EXPR_LEN];
    for (size_t i = 0; i < ents.size(); ++i) {
        char *ent = ents[i];
        if (strncmp(pre, ent, strlen(pre)) == 0) {
            struct stat fstat;
            strcpy(file, dir);
            strcat(file, ent);
            char *cddt = strcpy(new char[strlen(ent) + 2], ent);
            if (stat(file, &fstat) == 0 && S_ISDIR(fstat.st_mode) != 0) {
                cddt[strlen(cddt) + 1] = '\0';
                cddt[strlen(cddt)] = '/';
            }
            cddts.push_back(cddt);
        }
    }

    // free entries
    freebuf(ents);
} //}}}
//{{{ void CmdMgr::autoCmpltAppend()
void CmdMgr::autoCmpltAppend(int &nappend, const char *const pre,
                             const vector<char *> &cddts) {
    // append common prefix to current command string
    unsigned cmnLen = strcmn(cddts);
    if (cmnLen > strlen(pre)) {
        nappend = cmnLen - strlen(pre);
        memmove(input_ + csrPos_ + nappend, input_ + csrPos_,
                strlen(&input_[csrPos_]) + 1);
        strncpy(input_ + csrPos_, cddts[0] + strlen(pre), nappend);
        csrPos_ += nappend;
    }

    // if only one candidate and not directory, append white space
    if (cddts.size() == 1 && cddts[0][strlen(cddts[0]) - 1] != '/') {
        memmove(input_ + csrPos_ + 1, input_ + csrPos_,
                strlen(&input_[csrPos_]) + 1);
        input_[csrPos_] = ' ';
        csrPos_++;
        nappend++;
    }
} //}}}
//{{{ void CmdMgr::autoCmpltShow()
void CmdMgr::autoCmpltShow(const int &nappend, const vector<char *> &cddts) {
    if (cddts.size() < 2)
        return;

    unsigned maxLen = 0;
    for (size_t i = 0; i < cddts.size(); ++i)
        if (strlen(cddts[i]) > maxLen)
            maxLen = strlen(cddts[i]);
    maxLen += 2;

    printf("%s%s", VT100_CSRR, VT100_SCRU);
    fflush(stdout);
    int nfilePerLine = getWinCol() / maxLen;
    for (size_t i = 0; i < cddts.size(); ++i) {
        printf("%-*s", maxLen, cddts[i]);
        if ((i + 1) % nfilePerLine == 0)
            printf("\n");
    }
    if (cddts.size() % nfilePerLine != 0)
        printf("\n");
    int nlinePrev = maxPos_ / getWinCol();
    int nlineCur = (maxPos_ +  nappend) / getWinCol();
    for (int i = 0; i < nlinePrev - (nlineCur - nlinePrev); ++i)
        printf("%s", VT100_SCRU);
    fflush(stdout);
    printf("%s", VT100_CSRS);
    fflush(stdout);
} //}}}

//{{{ bool CmdMgr::read()
CmdMgr::Result CmdMgr::read() {
    setStdin();

    // initialize VT100 and save cursor position
    printf("%s%s", VT100_ARM_ON, VT100_CSRS);
    fflush(stdout);

    // initialize input string related variables
    memset(input_, '\0', EXPR_LEN);
    memset(inputBak_, '\0', EXPR_LEN);
    csrPos_ = 0;
    maxPos_ = strlen(prompt_);
    int ch = -1;
    refresh();

    // perform actions on every keystroke
    while (ch != ASCII_LF) {
        ch = getchar();
        // printable characters
        if (ch >= ASCII_MIN_PR && ch <= ASCII_MAX_PR) {
            memmove(input_ + csrPos_ + 1, input_ + csrPos_,
                    strlen(&input_[csrPos_]) + 1);
            input_[csrPos_] = ch;
            csrPos_++;
            refresh();
        }

        else if (ch < ASCII_MIN_PR || ch > ASCII_MAX_PR) {
            handleNonprintable(ch);
        }
        if (ch != ASCII_LF) // mask out all other not supported keys
            ch = -1;
    }
    printf("\n");

    resetStdin();

    return exec(input_);
}
//}}}
//{{{ bool CmdMgr::exec(const string)
CmdMgr::Result CmdMgr::exec(const char *const input) {
    char expr[EXPR_LEN];
    strcpy(expr, input);
    expandVar(expr);
    expandHome(expr);
    vector<char *> args;
    parse(args, expr);

    Result res = NOP;

    // set redirection
    bool redirected = false;
    int fd;
    fpos_t fpos;
    if (args.size() >= 2 && (strcmp(args[args.size() - 2], ">") == 0
                             || strcmp(args[args.size() - 2], ">>") == 0)) {
        char mode[2] = "w";
        if (strcmp(args[args.size() - 2], ">>") == 0)
            mode[0] = 'a';
        if (redirStdout(args[args.size() - 1], mode, fd, fpos))
            redirected = true;
        args.erase(args.end() - 2, args.end());
    }

    // execute command
    if (args.size() > 0) {
        his_.push_back(strcpy(new char[strlen(input) + 1], input));
        hisPtr_ = his_.size();
        Cmd *cmd = NULL;
        cmds_->lookup(args[0], cmd);

        if (!cmd) {
            strcpy(errorStr_, args[0]);
            error_ = E_NOT_REG;
            res = NOT_EXIST;
        }
        else {
            char *argv[args.size()];
            for (size_t i = 0; i < args.size(); ++i)
                argv[i] = args[i];
            int argc = (int)args.size();
            res = cmd->exec(argc, argv) ? (exit_ ? EXIT : SUCCESS) : FAIL;
        }
    }

    // reset stdout
    if (redirected)
        resetStdout(fd, fpos);

    // free
    freebuf(args);

    return res;
}
//}}}
//{{{ void CmdMgr::parse(vector<char *> &args, const char *const expr)
void CmdMgr::parse(vector<char *> &args, const char *const expr) const {
    // discard comment
    const char *pos = strstr(expr, comment_);
    int end = pos ? pos - expr : strlen(expr);

    bool inQuote = false;
    char arg[NAME_LEN];
    memset(arg, '\0', NAME_LEN);
    for (int i = 0; i < end; ++i) {
        int ch = expr[i];

        // '"' characters inside quotation marks are treated as one arg
        if (ch == '"')
            inQuote = ~inQuote;

        // '>' and ">>" for redirection
        if (ch == '>' && !inQuote) {
            if (strlen(arg) > 0) {
                args.push_back(strcpy(new char[strlen(arg) + 1], arg));
                memset(arg, '\0', NAME_LEN);
            }

            if (i + 1 < end && expr[i + 1] == '>') {
                args.push_back(strdup(">>"));
                i++;
            }
            else {
                args.push_back(strdup(">"));
                i++;
            }
            continue;
        }

        // delimiter handling
        if ((ch == ' ' || ch == '"' || ch == '\t' || ch == '\n')
            && !inQuote) {
            if (strlen(arg) > 0) {
                args.push_back(strcpy(new char[strlen(arg) + 1], arg));
                memset(arg, '\0', NAME_LEN);
            }
            continue;
        }

        // append other character to string
        if (ch != '"')
            arg[strlen(arg)] = ch;
    }

    if (strlen(arg) > 0)
        args.push_back(strcpy(new char[strlen(arg) + 1], arg));
}
//}}}

//{{{ void Cmd::usage(FILE *out)
void CmdMgr::usage(FILE *out) {
    unsigned maxLen = 0;
    StringHashNode<Cmd *> **bank = cmds_->head();
    for ( ; bank != cmds_->head() + cmds_->nbank(); bank++) {
        StringHashNode<Cmd *> *cur = (*bank);
        while (cur) {
            if (strlen(cur->key_) > maxLen)
                maxLen = strlen(cur->key_);
            cur = cur->next_;
        }
    }
    maxLen += 2;
    int count = 0;
    int ncmdPerLine = getWinCol() / maxLen;
    bank = cmds_->head();
    for ( ; bank != cmds_->head() + cmds_->nbank(); bank++) {
        StringHashNode<Cmd *> *cur = (*bank);
        while (cur) {
            fprintf(out, "%-*s", maxLen, cur->key_);
            if ((count + 1) % ncmdPerLine == 0)
                fprintf(out, "\n");
            cur = cur->next_;
            count++;
        }
    }
} //}}}


