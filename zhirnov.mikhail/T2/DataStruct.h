#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iomanip>
#include <iostream>
#include <string>
#include <complex>

namespace nspace
{
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  struct DelimiterIO { char exp; };
  struct DoubleIO { double& ref; };
  struct UllIO { unsigned long long& ref; };
  struct StringIO { std::string& ref; };
  struct LabelIO { std::string exp; };
  struct CharIO { char& ref; };

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
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, CharIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

  bool compareDataStruct(const DataStruct& a, const DataStruct& b);
}
#endif

