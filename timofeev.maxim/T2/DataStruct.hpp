#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <cmath>
#include <algorithm>

namespace timofeev {
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct UllBinIO
  {
    unsigned long long& ref;
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
  std::istream& operator>>(std::istream& in, UllBinIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

  bool has_e(std::string& check);
  std::string beautyDouble(double check);
  std::string utos(unsigned long long key);
  bool dataStructCompare(const DataStruct& a, const DataStruct& b);
}

#endif
