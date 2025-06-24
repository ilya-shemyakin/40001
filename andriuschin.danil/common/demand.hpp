#ifndef DEMAND_HPP
#define DEMAND_HPP

#include <iosfwd>

namespace andriuschin
{
  struct Demand
  {
    char expected = '\0';
  };
  std::istream& operator>>(std::istream& in, Demand&& value);
}

#endif
