#include "data_struct.hpp"

#include <iostream>
#include <iomanip>
#include <cstring>

#include "format_guard.hpp"
#include "literals.hpp"

std::ostream& andriuschin::operator<<(std::ostream& out, const andriuschin::DataStruct& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << "(:key1 " << CharLiteral(value.key1) << ":key2 " << RationalLiteral(value.key2)
      << ":key3 " << StringLiteral(value.key3) << ":)";
}
std::istream& andriuschin::operator>>(std::istream& in, DataStruct& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  FormatGuard guard(in);
  constexpr size_t nKeys = 3;
  constexpr size_t keyLen = 4 + 1;
  char prefix[keyLen] = "\0";
  bool wasRecived[nKeys] = {};
  if (!(in >> Demand{'('} >> std::noskipws))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  while ((in >> std::setw(keyLen) >> prefix) && (std::strcmp(prefix, ":key") == 0))
  {
    in >> prefix[0] >> Demand{' '};
    if (!in || prefix[0] < '0' || prefix[0] > '3' || wasRecived[prefix[0] - '1']) {
      in.setstate(std::ios::failbit);
      return in;
    }
    wasRecived[prefix[0] - '1'] = true;
    switch (prefix[0])
    {
    case '1':
      in >> CharLiteral(value.key1);
      break;
    case '2':
      in >> RationalLiteral(value.key2);
      break;
    case '3':
      in >> StringLiteral(value.key3);
      break;
    }
    if (wasRecived[0] && wasRecived[1] && wasRecived[2])
    {
      break;
    }
  }
  return in >> Demand{':'} >> std::noskipws >> Demand{')'};
}

bool andriuschin::DsCompare::operator()(const DataStruct& lhs, const DataStruct& rhs) const
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  else if (lhs.key2 != rhs.key2)
  {
    return (static_cast< double >(lhs.key2.first) / lhs.key2.second)
           < (static_cast< double >(rhs.key2.first) / rhs.key2.second);
  }
  else
  {
    return lhs.key3.size() < rhs.key3.size();
  }
}

std::istream& andriuschin::operator>>(std::istream& in, andriuschin::Demand&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '\0';
  if (!(in >> c) || (c != value.expected))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
