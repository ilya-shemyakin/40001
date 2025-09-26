#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <ios>

namespace trojan
{
    class StreamGuard
    {
    public:
        StreamGuard(std::basic_ios< char >& stream);
        ~StreamGuard();
    private:
        std::basic_ios< char >& stream_;
        char fill_;
        std::streamsize precision_;
        std::streamsize width_;
        std::basic_ios< char >::fmtflags flags_;
    };
}

#endif
