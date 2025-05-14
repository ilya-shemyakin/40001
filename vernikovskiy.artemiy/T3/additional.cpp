#include "additional.h"

namespace workable
{
    std::istream& operator>>(std::istream& is, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(is);
        if (!sentry)
        {
            return is;
            std::cout << "SENTRY" << std::endl;
        }
        char c = '0';
        is >> c;
        if (is && (c != dest.delim))
        {
            is.setstate(std::ios::failbit);
        }
        return is;
    }

    std::ifstream& operator>>(std::ifstream& is, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(is);
        if (!sentry)
        {
            return is;
            std::cout << "SENTRY" << std::endl;
        }
        char c = '0';
        is >> c;
        if (is.good() && (c != dest.delim))
        {
            is.setstate(std::ios::failbit);
        }
        return is;
    }
}
