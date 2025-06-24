#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

#include <ios>

class StreamGuard {
private:
    std::basic_ios<char> &s_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags flags_;
public:
    StreamGuard(std::basic_ios<char> &s);
    ~StreamGuard();

};


#endif
