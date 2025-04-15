#ifndef NAMESPACE_HPP
#define NAMESPACE_HPP

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

  bool dataStructCompare(const DataStruct& a, const DataStruct& b);

  std::string beautyDouble(double check);

  bool has_e(std::string check);

  std::string utos(unsigned long long key);

  bool parsingString(std::istream& in, std::string& key3);

  bool parsingDouble(std::istream& in, double& key1);

  bool parsingUll(std::istream& in, unsigned long long& key2);

  std::istream& operator >> (std::istream& in, DataStruct& data);

  std::ostream& operator<< (std::ostream& out, const DataStruct& data);
}

#endif
