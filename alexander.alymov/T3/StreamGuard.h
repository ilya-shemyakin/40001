#ifndef STREAMGUARD_H
#define STREAMGUARD_H
#include <iostream>

namespace alymov
{
    class StreamGuard
    {
    public:
        StreamGuard(std::basic_ios< char >& s);
        ~StreamGuard();
    private:
        std::basic_ios< char >& s_;
        std::streamsize precision_;
        std::basic_ios< char >::fmtflags fmt_;
    };
}

#endif
