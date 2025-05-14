#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <iostream>
namespace averin
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

    struct ULLBinIO
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
    std::istream& operator>>(std::istream& in, ULLBinIO&& dest);
    std::istream& operator>>(std::istream& in, ULLLitIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

    bool dataStructCompare(const DataStruct& a, const DataStruct& b);
}
#endif
