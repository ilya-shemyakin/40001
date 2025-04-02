#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

#include <ios>

class StreamGuard {
private:
    std::basic_ios<char> &s_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags flags_;
public:
    StreamGuard(std::basic_ios<char> &s) :
        s_(s),
        precision_(s.precision()),
        flags_(s.flags())
    {}
    ~StreamGuard() {
        s_.precision(precision_);
        s_.flags(flags_);
    }

};


#endif
