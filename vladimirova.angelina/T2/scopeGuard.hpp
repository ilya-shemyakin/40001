#ifndef SCOPEGUARD_HPP
#define SCOPEGUARD_HPP

#include <ios>
#include <iomanip>

namespace vladimirova
{
    class iofmtguard
    {
    public:
        explicit iofmtguard(std::basic_ios<char>& stream);
        ~iofmtguard();
    private:
        std::basic_ios<char>& stream_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::ios::fmtflags flags_;
    };
}

#endif
