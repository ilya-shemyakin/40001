#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <utility>
#include <string>
#include <iosfwd>

namespace andriuschin
{
  struct DataStruct
  {
    char key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };
  std::ostream& operator<<(std::ostream& out, const DataStruct& value);
  std::istream& operator>>(std::istream& in, DataStruct& value);

  struct DsCompare
  {
    bool operator()(const DataStruct& lhs, const DataStruct& rhs) const;
  };

  struct Demand
  {
    char expected = '\0';
  };
  std::istream& operator>>(std::istream& in, Demand&& value);
}

#endif
