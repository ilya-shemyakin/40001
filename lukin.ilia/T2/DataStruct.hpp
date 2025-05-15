#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <complex>
#include <string>
#include <iostream>

struct DataStruct {
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
};

bool parseULL(const std::string& s, unsigned long long& value);
bool parseComplex(const std::string& s, std::complex<double>& value);
bool parseDataStruct(const std::string& line, DataStruct& ds);
bool compareDataStruct(const DataStruct& a, const DataStruct& b);
std::ostream& operator<<(std::ostream& os, const DataStruct& ds);

#endif // DATASTRUCT_HPP