#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <complex>
#include <string>
#include <iomanip>
#include <map>
#include "StreamGuard.h"

namespace alymov
{

    struct DataStruct {
        unsigned long long key1;
        std::complex<double> key2;
        std::string key3;
    };

    struct DelimiterIO {
        char exp;
    };

    struct ULLHexIO {
        unsigned long long& ref;
    };

    struct ComplexIO {
        std::complex<double>& ref;
    };

    struct StringIO {
        std::string& ref;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, ULLHexIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, ComplexIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

    bool compareDataStruct(const DataStruct& a, const DataStruct& b);
}

#endif
