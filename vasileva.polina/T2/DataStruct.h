#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <iostream>
#include "iofmtguard.h"
namespace vasileva
{
    struct DataStruct
    {
        unsigned long long key1;
        unsigned long long key2;
        std::string key3;
    };
    bool compareDataStruct(const DataStruct& a, const DataStruct& b);

    struct DelimiterIO
    {
        char exp;
    };

    struct ULLOctIO
    {
        unsigned long long& ref;
    };

    struct ULLLitIO
    {
        unsigned long long& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    struct LabelIO
    {
        std::string exp;
    };
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, ULLOctIO&& dest);
    std::istream& operator>>(std::istream& in, ULLLitIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);


}
#endif
