#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include <iostream>

namespace workable
{
    struct Wrapper {
        std::istream& cin;
        std::ostream& cout;
        Wrapper(std::istream& cin, std::ostream& cout) : cin(cin), cout(cout) {};
    };
    struct DelimiterIO {
        char delim;
    };
    
    std::istream& operator>>(std::istream& is, DelimiterIO&& delim);
};

#endif
