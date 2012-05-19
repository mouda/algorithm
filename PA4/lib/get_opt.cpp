// **************************************************************************
// File       [ get_opt.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ parse options and arguments ]
// Date       [ Ver 1.0 started 2010/03/30 ]
// **************************************************************************

#include <sys/ioctl.h> // getting window size

#include "get_opt.h"

using namespace std;
using namespace CommonNs;

bool OptMgr::regOpt(Opt * const opt) {
    // checking for flags
    if (opt->flags_.size() == 0) {
        strcpy(errorStr_, "No option flags");
        error_ = E_REQ;
        return false;
    }

    // checking for existing flags
    for (size_t i = 0; i < flags_.size(); ++i) {
        for (size_t j = 0; j < opt->flags_.size(); ++j) {
            if (strcmp(flags_[i], opt->flags_[j]) == 0) {
                strcpy(errorStr_, "Flag already exist");
                error_ = E_EXIST;
                return false;
            }
        }
    }

    // add option
    opts_.push_back(opt);
    parsedOpts_.push_back(false);
    parsedValues_.push_back(NULL);

    // add flags
    for (size_t i = 0; i < opt->flags_.size(); ++i) {
        char *f = opt->flags_[i];
        flags_.push_back(strcpy(new char[strlen(f) + 1], f));
        flagToOpt_.push_back(opts_.size() - 1);
    }

    return true;
}

bool OptMgr::parse(int argc, char **argv) {
    // clear previous parsed data
    for (size_t i = 0; i < parsedArgs_.size(); ++i)
        delete [] parsedArgs_[i];
    parsedArgs_.clear();
    for (size_t i = 0; i < opts_.size(); ++i) {
        parsedOpts_[i] = false;
        delete [] parsedValues_[i];
        parsedValues_[i] = NULL;
    }

    // keep first argument
    if (keepFirst_ && argc >= 1)
        parsedArgs_.push_back(strcpy(new char[strlen(argv[0]) + 1], argv[0]));

    for (int i = 1; i < argc; ++i) {
        bool lastArg = i == argc - 1 ? true : false;
        if (strlen(argv[i]) >= 2 && argv[i][0] == '-') { // options
            if (argv[i][1] != '-') {
                if(!parseShortFlags(&argv[i][1], i, lastArg, argv))
                    return false;
            }
            else {
                if (!parseLongFlag(&argv[i][2], i, lastArg, argv))
                    return false;
            }
        }
        else { // not option
            parsedArgs_.push_back(strcpy(new char[strlen(argv[i]) + 1],
                                         argv[i]));
        }
    }
    return true;
}

//{{{ bool OptMgr::parseShortFlags()
bool OptMgr::parseShortFlags(const char * const flags
    , int &i
    , const bool &lastArg
    , char *argv[])
{
    for (size_t j = 0; j < strlen(flags); ++j) {
        bool lastFlag = j == strlen(flags) - 1 ? true : false;

        // find flag index
        size_t k = 0;
        for ( ; k < flags_.size(); ++k)
            if (strlen(flags_[k]) == 1 && flags[j] == flags_[k][0])
                break;

        // check option registration
        if (k == flags_.size()) {
            strcpy(errorStr_, "Option not registered");
            error_ = E_NOT_REG;
            return false;
        }

        // check option requirement
        Opt *opt = opts_[flagToOpt_[k]];
        bool nextIsOpt = false;
        if (!lastArg)
            nextIsOpt = argv[i + 1][0] == '-' ? true : false;
        if (opt->type_ == Opt::STR_REQ && lastFlag && (nextIsOpt || lastArg)) {
            strcpy(errorStr_, "Option requires value");
            error_ = E_REQ;
            return false;
        }

        // assign option
        parsedOpts_[flagToOpt_[k]] = true;

        // assign value
        if (opt->type_ == Opt::STR_REQ
            || (opt->type_ == Opt::STR_OPT && lastFlag && !lastArg
                && !nextIsOpt)) {
            if (lastFlag) {
                i++;
                char *val = strcpy(new char[strlen(argv[i]) + 1], argv[i]);
                parsedValues_[flagToOpt_[k]] = val;
            }
            else {
                j++;
                char *val = strdup(&flags[j]);
                parsedValues_[flagToOpt_[k]] = val;
            }
            return true;
        }
    }
    return true;
} //}}}
//{{{ bool OptMgr::parseLongFlag()
bool OptMgr::parseLongFlag(const char * const flag, int &i,
                           const bool &lastArg, char *argv[]) {
    // check for value assignment via equal sign
    const char *equ = strchr(flag, '=');
    int pos = equ ? equ - flag : strlen(flag);

    size_t j = 0;
    for ( ; j < flags_.size(); ++j)
        if (strncmp(flag, flags_[j], pos) == 0)
            break;

    // check option existence
    if (j == flags_.size() || pos < 2) {
        strcpy(errorStr_, "Option not registered");
        error_ = E_NOT_REG;
        return false;
    }

    // check option requirement
    Opt *opt = opts_[flagToOpt_[j]];
    bool nextIsOpt = false;
    bool isreq = opt->type_ == Opt::STR_REQ;
    bool isopt = opt->type_ == Opt::STR_OPT;
    if (!lastArg)
        nextIsOpt = argv[i + 1][0] == '-' ? true : false;
    if (isreq && !equ && (lastArg || nextIsOpt)) {
        strcpy(errorStr_, "Option requires value");
        error_ = E_REQ;
        return false;
    }

    // assign option
    parsedOpts_[flagToOpt_[j]] = true;

    // assign value
    if (isreq || ((isopt && equ) || (!lastArg && !nextIsOpt))) {
        char *val;
        if (equ)
            val = strcpy(new char[strlen(&flag[pos]) + 1], &flag[pos]);
        else {
            i++;
            val = strcpy(new char[strlen(argv[i]) + 1], argv[i]);
        }
        parsedValues_[flagToOpt_[j]] = val;
    }

    return true;
} //}}}


// **************************************************************************
// Function   [ OptMgr::usage(ostream &) ]
// Author     [ littleshamoo ]
// Synopsis   [ print the usage of the program in the following format
//              (similar to the MAN page in linux system)
//
//              NAME
//                      <program name> - <program short description>
//
//              SYNOPSIS
//                      <program name> [options] <arguments>
//
//              DESCRIPTION
//                      <program description>
//
//              ARGUMENTS
//                      <arg1>  <arg1 description>
//                      <arg2>  <arg2 description>
//                      <arg3>  <arg3 description>
//                      ...
//
//              OPTIONS
//                      <opt1> <opt1 description>
//                      <opt2> <opt2 description>
//                      <opt3> <opt3 description>
//                      ...
//            ]
// **************************************************************************
//{{{ void OptMgr::usage(FILE *out)
void OptMgr::usage(FILE *out) {
    size_t wsCol   = WS_COL;
    size_t tabSize = TAB_SIZE;

    // determine window size
    struct winsize ws;
    wsCol = ioctl(0, TIOCGWINSZ, &ws) == 0 ? ws.ws_col - 2 : wsCol;

    // minimum line width is three tabs
    wsCol = wsCol < 3 * tabSize ? 3 * tabSize : wsCol;

    nameUsage(out, wsCol, tabSize);
    synopsisUsage(out, wsCol, tabSize);
    descriptionUsage(out, wsCol, tabSize);
    argumentUsage(out, wsCol, tabSize);
    optionUsage(out, wsCol, tabSize);
}
//}}}

//{{{ void OptMgr::nameUsage()
void OptMgr::nameUsage(FILE *out, const size_t &wsCol,
                       const size_t &tabSize) {
    vector<char *> bufs;
    char buf[DES_LEN];

    fprintf(out, "\nNAME\n");
    strcpy(buf, name_);
    strcat(buf, " - ");
    if (shortDes_)
        strcat(buf, shortDes_);
    split(bufs, buf, wsCol - tabSize);
    for (size_t i = 0; i < bufs.size(); ++i) {
        for (size_t j = 0; j < tabSize; ++j)
            fprintf(out, " ");
        fprintf(out, "%s\n", bufs[i]);
    }
    fprintf(out, "\n");
    freebuf(bufs);
} //}}}
//{{{ void OptMgr::synopsisUsage()
void OptMgr::synopsisUsage(FILE *out, const size_t &wsCol,
                           const size_t &tabSize) {
    fprintf(out, "SYNOPSIS\n");
    char sFlag[NAME_LEN];
    char lFlag[DES_LEN];
    sFlag[0] = '\0';
    lFlag[0] = '\0';
    for (size_t i = 0; i < flags_.size(); ++i) {
        Opt *opt = opts_[flagToOpt_[i]];
        switch (opt->type_) {
            case Opt::BOOL:
                if (strlen(flags_[i]) == 1) {
                    if (strlen(sFlag) == 0)
                        strcpy(sFlag, "[-");
                    strcat(sFlag, flags_[i]);
                }
                else {
                    strcat(lFlag, "[--");
                    strcat(lFlag, flags_[i]);
                    strcat(lFlag, "] ");
                }
                break;
            case Opt::STR_REQ:
                strcat(lFlag, "[-");
                if (strlen(flags_[i]) > 1)
                    strcat(lFlag, "-");
                strcat(lFlag, flags_[i]);
                strcat(lFlag, " ");
                strcat(lFlag, opt->meta_);
                strcat(lFlag, "] ");
                break;
            case Opt::STR_OPT:
                strcat(lFlag, "[-");
                if (strlen(flags_[i]) > 1)
                    strcat(lFlag, "-");
                strcat(lFlag, flags_[i]);
                strcat(lFlag, " [");
                strcat(lFlag, opt->meta_);
                strcat(lFlag, "]] ");
                break;
        }
    }
    if (strlen(sFlag) > 0)
        strcat(sFlag, "] ");
    for (size_t i = 0; i < args_.size(); ++i) {
        switch (args_[i]->type_) {
            case Arg::REQ:
                strcat(lFlag, args_[i]->meta_);
                strcat(lFlag, " ");
                break;
            case Arg::OPT:
                strcat(lFlag, "[");
                strcat(lFlag, args_[i]->meta_);
                strcat(lFlag, "] ");
                break;
            case Arg::REQ_INF:
                strcat(lFlag, args_[i]->meta_);
                strcat(lFlag, "... ");
                break;
            case Arg::OPT_INF:
                strcat(lFlag, "[");
                strcat(lFlag, args_[i]->meta_);
                strcat(lFlag, "]... ");
                break;
        }
    }
    char buf[DES_LEN];
    strcpy(buf, name_);
    strcat(buf, " ");
    strcat(buf, sFlag);
    strcat(buf, lFlag);

    vector<char *> bufs;
    split(bufs, buf, wsCol - tabSize, true, " ");
    for (size_t i = 0; i < bufs.size(); ++i) {
        for (size_t j = 0; j < tabSize; ++j)
            fprintf(out, " ");
        fprintf(out, "%s\n", bufs[i]);
    }
    fprintf(out, "\n");
    freebuf(bufs);
} //}}}
//{{{ void OptMgr::descriptionUsage()
void OptMgr::descriptionUsage(FILE *out, const size_t &wsCol,
                              const size_t &tabSize) {
    vector<char *> bufs;
    fprintf(out, "DESCRIPTION\n");
    if (!des_ || strcmp(des_, "") == 0) {
        for (size_t i = 0; i < tabSize; ++i)
            fprintf(out, " ");
        fprintf(out, "NONE\n");
    }
    else {
        split(bufs, des_, wsCol - tabSize);
        for (size_t i = 0; i < bufs.size(); ++i) {
            for (size_t j = 0; j < tabSize; ++j)
                fprintf(out, " ");
            fprintf(out, "%s\n", bufs[i]);
        }
    }
    fprintf(out, "\n");
    freebuf(bufs);
} //}}}
//{{{ void OptMgr::argumentUsage()
void OptMgr::argumentUsage(FILE *out, const size_t &wsCol,
                           const size_t &tabSize) {
    if (args_.size() != 0)
        fprintf(out, "ARGUMENTS\n");
    for (size_t i = 0; i < args_.size(); ++i) {
        Arg *arg = args_[i];
        for (size_t j = 0; j < tabSize; ++j)
            fprintf(out, " ");
        fprintf(out, "%s", arg->meta_);
        bool nextline = strlen(arg->meta_) + 2 >= tabSize;
        if (nextline)
            fprintf(out, "\n");
        else
            for (size_t j = strlen(arg->meta_); j < tabSize; ++j)
                fprintf(out, " ");

        vector<char *> bufs;
        split(bufs, arg->des_, wsCol - 2 * tabSize);
        for (size_t j = 0; j < bufs.size(); ++j) {
            if (j != 0 || nextline)
                for (size_t k = 0; k < 2 * tabSize; ++k)
                    fprintf(out, " ");
            fprintf(out, "%s\n", bufs[j]);
        }
        fprintf(out, "\n");
        freebuf(bufs);
    }
} //}}}
//{{{ void OptMgr::optionUsage()
void OptMgr::optionUsage(FILE *out, const size_t &wsCol,
                         const size_t &tabSize) {
    if (opts_.size() > 0)
        fprintf(out, "OPTION\n");
    char flags[NAME_LEN];
    for (size_t i = 0; i < opts_.size(); ++i) {
        flags[0] = '\0';
        Opt *opt = opts_[i];
        for (size_t j = 0; j < opt->flags_.size(); ++j) {
            if (strlen(opt->flags_[j]) == 1) {
                strcat(flags, "-");
                strcat(flags, opt->flags_[j]);
            }
            else {
                strcat(flags, "--");
                strcat(flags, opt->flags_[j]);
            }
            if (j + 1 != opt->flags_.size())
                strcat(flags, ",");
        }
        if (opt->type_ == Opt::STR_REQ) {
            strcat(flags, " ");
            strcat(flags, opt->meta_);
        }
        else if (opt->type_ == Opt::STR_OPT) {
            strcat(flags, " [");
            strcat(flags, opt->meta_);
            strcat(flags, "]");
        }

        for (size_t j = 0; j < tabSize; ++j)
            fprintf(out, " ");
        fprintf(out, "%s", flags);
        bool nextline = strlen(flags) + 2 >= tabSize;
        if (nextline)
            fprintf(out, "\n");
        else
            for (size_t j = strlen(flags); j < tabSize; ++j)
                fprintf(out, " ");

        vector<char *> bufs;
        split(bufs, opt->des_, wsCol - 2 * tabSize);
        for (size_t j = 0; j < bufs.size(); ++j) {
            if (j != 0 || nextline)
                for (size_t k = 0; k < 2 * tabSize; ++k)
                    fprintf(out, " ");
            fprintf(out, "%s\n", bufs[j]);
        }
        fprintf(out, "\n");
        freebuf(bufs);
    }
} //}}}

//{{{ void OptMgr::split()
void OptMgr::split(vector<char *> &result, const char * const input,
                   const size_t &col, const bool &fit,
                   const char * const sep) {
    int begin = 0;
    while (begin + col < strlen(input)) {
        char *buf = strncpy(new char[col + 1], &input[begin], col);
        buf[col] = '\0';
        if (strchr(sep, input[begin + col]))
            begin += col + 1;
        else {
            int len = 0;
            for (size_t i = 0; i < strlen(sep); ++i)
                if (char *pos = strrchr(buf, sep[i]))
                    if (pos - buf > len)
                        len = pos - buf;
            if (len == 0)
                len = col - 1;
            buf[len + 1] = '\0';
            begin += len;
        }
        if (fit)
            buf = this->fit(buf, col);
        result.push_back(buf);
    }
    while (input[begin] == ' ')
        begin++;
    result.push_back(strcpy(new char[strlen(input) - begin + 1],
                            &input[begin]));
} //}}}
//{{{ char *OptMgr::fit()
char *OptMgr::fit(char *input, const size_t &col) {
    // find begining
    char *begin = input;
    while ((*begin) == ' ' && (*begin) != '\0')
        begin++;

    // find end
    char *end = strchr(input, '\0') - 1;
    while ((*end) == ' ' && end != input)
        end--;
    end++;

    // count number of spaces
    int nwht = 0;
    char *cur = begin;
    while (cur != end) {
        if ((*cur) == ' ')
            nwht++;
        cur++;
    }

    // find out how many to fill
    int nfill = col - (end - begin);
    int nadd = nfill / nwht;
    int nleft = nfill % nwht;

    // fill white
    char *buf = new char[col + 1];
    buf[col] = '\0';
    int whtNo = 0;
    int bufptr = 0;
    while (begin != end) {
        buf[bufptr] = (*begin);
        bufptr++;
        if ((*begin) == ' ') {
            int add = whtNo < nleft ? 1 : 0;
            add += nadd;
            for (int i = 0; i < add; ++i, ++bufptr)
                buf[bufptr] = ' ';
            whtNo++;
        }
        begin++;
    }

    strcpy(input, buf);
    delete [] buf;
    return input;
} //}}}

