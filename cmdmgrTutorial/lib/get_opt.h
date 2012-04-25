// **************************************************************************
// File       [ get_opt.h ]
// Author     [ littleshamoo ]
// Synopsis   [ parse options and arguments ]
// Date       [ Ver 1.0 started 2010/03/30 ]
// **************************************************************************


#ifndef _COMMON_GET_OPT_H_
#define _COMMON_GET_OPT_H_

#include <cstring>
#include <cstdio>
#include <vector>
#include <string>

namespace CommonNs {

//{{{ struct Arg
struct Arg {
    enum Type { REQ = 0, OPT, REQ_INF, OPT_INF };

         Arg(const Type         &type,
             const char * const des,
             const char * const meta);
         ~Arg();

    Type type_;
    char *des_;
    char *meta_;
}; //}}}
//{{{ struct Opt
struct Opt {
    enum                Type { BOOL = 0, STR_REQ, STR_OPT };

                        Opt(const Type         &type,
                            const char * const des,
                            const char * const meta);
                        ~Opt();

    bool                addFlag(const char * const f);

    Type                type_;
    char                *des_;
    char                *meta_;
    std::vector<char *> flags_;
}; //}}}

//{{{ class OptMgr
class OptMgr {
public:
    enum       Error { E_EXIST = 0, E_NOT_REG, E_REQ };

               OptMgr();
               ~OptMgr();

    // program information
    const char *getName() const;
    void       setName(const char * const name);
    void       setShortDes(const char * const shortDes);
    void       setDes(const char * const des);

    // register
    bool       regArg(Arg * const arg);
    bool       regOpt(Opt * const opt);

    // parse
    void       keepFirst(const bool &keep);
    bool       parse(int argc, char **argv);

    // registered contents
    size_t     getNFlag() const;
    char       *getFlag(const size_t &i) const;
    size_t     getNOpt() const;
    Opt        *getOpt(const size_t &i) const;
    Opt        *getOpt(const char * const f) const;
    size_t     getNArg() const;
    Arg        *getArg(const size_t &i) const;

    // parsed result
    size_t     getNParsedArg() const;
    const char *getParsedArg(const size_t &i) const;
    bool       getParsedOpt(const char * const f) const;
    bool       getParsedOpt(const size_t &i) const;
    char       *getParsedValue(const char * const f) const;
    bool       getParsedValue(const size_t &i) const;
    Error      getError() const;
    const char *getErrorStr() const;

    // usage
    void       usage(FILE *out = stdout);

private:
    static const size_t WS_COL   = 78;
    static const size_t TAB_SIZE = 8;
    static const size_t NAME_LEN = 256;
    static const size_t DES_LEN  = 4096;

    bool                keepFirst_;
    char                name_[NAME_LEN];
    char                *shortDes_;
    char                *des_;
    Error               error_;
    char                errorStr_[NAME_LEN];

    std::vector<char *> flags_;
    std::vector<Arg *>  args_;
    std::vector<Opt *>  opts_;
    std::vector<size_t> flagToOpt_;
    std::vector<char *> parsedArgs_;
    std::vector<bool>   parsedOpts_;
    std::vector<char *> parsedValues_;

    // used by parsing
    bool                parseShortFlags(const char * const flags, int &i,
                                        const bool &lastArg, char *argv[]);
    bool                parseLongFlag(const char * const flags, int &i,
                                      const bool &lastArg, char *argv[]);

    // used by usage
    void                nameUsage(FILE *out, const size_t &wsCol,
                                  const size_t &tabSize);
    void                synopsisUsage(FILE *out, const size_t &wsCol,
                                      const size_t &tabSize);
    void                descriptionUsage(FILE *out, const size_t &wsCol,
                                         const size_t &tabSize);
    void                argumentUsage(FILE *out, const size_t &wsCol,
                                      const size_t &tabSize);
    void                optionUsage(FILE *out, const size_t &wsCol,
                                    const size_t &tabSize);
    void                split(std::vector<char *> &result,
                              const char * const input, const size_t &col,
                              const bool &fit = true,
                              const char * const sep = "- ");
    char                *fit(char *input, const size_t &col);
    void                freebuf(std::vector<char *> &buf);
}; //}}}

// inline methods
//{{{ struct Arg
inline Arg::Arg(const Type &type, const char * const des,
                const char * const meta) {
    type_ = type;
    des_ = strcpy(new char[strlen(des) + 1], des);
    meta_ = strcpy(new char[strlen(meta) + 1], meta);
}

inline Arg::~Arg() {
    delete [] des_;
    delete [] meta_;
}
//}}}
//{{{ struct Opt
inline Opt::Opt(const Type &type, const char * const des,
                const char * const meta) {
    type_ = type;
    des_ = strcpy(new char[strlen(des) + 1], des);
    meta_ = strcpy(new char[strlen(meta) + 1], meta);
}

inline Opt::~Opt() {
    delete [] des_;
    delete [] meta_;
}

inline bool Opt::addFlag(const char * const f) {
    for (size_t i = 0; i < flags_.size(); ++i)
        if (strcmp(flags_[i], f) == 0)
            return false;
    flags_.push_back(strcpy(new char[strlen(f) + 1], f));
    return true;
}
//}}}

//{{{ class OptMgr
inline OptMgr::OptMgr() {
    keepFirst_   = false;
    name_[0]     = '\0';
    des_         = NULL;
    shortDes_    = NULL;
    error_       = E_EXIST;
    errorStr_[0] = '\0';
}

inline OptMgr::~OptMgr() {
    delete [] des_;
    delete [] shortDes_;
    for (size_t i = 0; i < parsedArgs_.size(); ++i)
        delete [] parsedArgs_[i];
    for (size_t i = 0; i < parsedValues_.size(); ++i)
        delete [] parsedValues_[i];
}

inline const char *OptMgr::getName() const {
    return name_;
}

inline void OptMgr::setName(const char * const name) {
    strncpy(name_, name, NAME_LEN);
}

inline void OptMgr::setShortDes(const char * const shortDes) {
    shortDes_ = strcpy(new char[strlen(shortDes) + 1], shortDes);
}

inline void OptMgr::setDes(const char * const des) {
    des_ = strcpy(new char[strlen(des) + 1], des);
}

inline bool OptMgr::regArg(Arg * const arg) {
    args_.push_back(arg);
    return true;
}

inline void OptMgr::keepFirst(const bool &keep) {
    keepFirst_ = keep;
}

inline size_t OptMgr::getNFlag() const {
    return flags_.size();
}

inline char *OptMgr::getFlag(const size_t &i) const {
    return flags_[i];
}

inline size_t OptMgr::getNOpt() const {
    return opts_.size();
}

inline Opt *OptMgr::getOpt(const size_t &i) const {
    return opts_[i];
}

inline Opt *OptMgr::getOpt(const char * const f) const {
    for (size_t i = 0; i < flags_.size(); ++i)
        if (strcmp(flags_[i], f) == 0)
            return opts_[flagToOpt_[i]];
    return NULL;
}

inline size_t OptMgr::getNArg() const {
    return args_.size();
}

inline Arg *OptMgr::getArg(const size_t &i) const {
    return args_[i];
}

inline size_t OptMgr::getNParsedArg() const {
    return parsedArgs_.size();
}

inline const char *OptMgr::getParsedArg(const size_t &i) const {
    return parsedArgs_[i];
}

inline bool OptMgr::getParsedOpt(const char * const f) const {
    for (size_t i = 0; i < flags_.size(); ++i)
        if (strcmp(flags_[i], f) == 0)
            return parsedOpts_[flagToOpt_[i]];
    return false;
}

inline bool OptMgr::getParsedOpt(const size_t &i) const {
    return parsedOpts_[i];
}

inline char *OptMgr::getParsedValue(const char * const f) const {
    for (size_t i = 0; i < flags_.size(); ++i)
        if (strcmp(flags_[i], f) == 0)
            return parsedValues_[flagToOpt_[i]];
    return NULL;
}

inline bool OptMgr::getParsedValue(const size_t &i) const {
    return parsedValues_[i];
}


inline OptMgr::Error OptMgr::getError() const {
    return error_;
}

inline const char *OptMgr::getErrorStr() const {
    return errorStr_;
}

inline void OptMgr::freebuf(std::vector<char *> &buf) {
    for (size_t i = 0; i < buf.size(); ++i)
        delete [] buf[i];
    buf.clear();
}
//}}}

};


#endif

