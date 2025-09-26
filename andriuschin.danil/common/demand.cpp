#include "demand.hpp"

#include <istream>

std::istream& andriuschin::operator>>(std::istream& in, andriuschin::Demand&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '\0';
  if (!(in.get(c)) || (c != value.expected))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
