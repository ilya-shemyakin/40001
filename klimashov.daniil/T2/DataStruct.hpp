#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <cmath>
#include <algorithm>
#include <complex>
#include <iostream>

namespace nspace {

struct DataStruct
{
    long long key1 = 0;
    std::complex<double> key2 = {0.0, 0.0};
    std::string key3;
};

struct DelimiterIO { char exp; };
struct SllIO { long long& ref; };
struct ComplexIO { std::complex<double>& ref; };
struct StringIO { std::string& ref; };

class iofmtguard {
public:
    iofmtguard(std::basic_ios<char>& s);
    ~iofmtguard();
private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, SllIO&& dest);
std::istream& operator>>(std::istream& in, ComplexIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

bool dataStructCompare(const DataStruct& a, const DataStruct& b);
std::string formatDouble(double value);

}

#endif
