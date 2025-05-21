#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <cmath>
#include <regex>
#include <algorithm>

namespace dolzhenkov {
    struct DataStruct
    {
        double key1;
        long long key2;
        std::string key3;
    };

    struct DelimiterIO
    {
        char exp;
    };

    struct SllLitIO
    {
        long long& ref;
    };

    struct DoubleIO
    {
        double& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    class iofmtguard
    {
    public:
        iofmtguard(std::basic_ios< char >& s);
        ~iofmtguard();
    private:
        std::basic_ios< char >& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios< char >::fmtflags fmt_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, SllLitIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

    std::string beautyDouble(double check);
    bool dataStructCompare(const DataStruct& a, const DataStruct& b);
    bool isDoubleSCI(const std::string& str);
}

#endif
