#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <iostream>
#include <string>

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

struct DoubleIO
{
    double& ref;
};

struct LongLongIO
{
    long long& ref;
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
    iofmtguard(std::basic_ios<char>& s);
    ~iofmtguard();
private:
    std::basic_ios<char>& s_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, DoubleIO&& dest);
std::istream& operator>>(std::istream& in, LongLongIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, LabelIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
bool compareDataStructs(const DataStruct& a, const DataStruct& b);

#endif
