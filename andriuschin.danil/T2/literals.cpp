#include "data_struct.hpp"

#include <iostream>

#include "format_guard.hpp"
#include "literals.hpp"

template<>
std::ostream& andriuschin::operator<<< andriuschin::CharLiteral::value_type,
      andriuschin::CharLiteral::id >(std::ostream& out, const CharLiteral&& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << '\'' << value.link_ << '\'';
}
template<>
std::istream& andriuschin::operator>>< andriuschin::CharLiteral::value_type,
      andriuschin::CharLiteral::id >(std::istream& in, CharLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  FormatGuard guard(in);
  char c = '\0';
  if (in >> Demand{'\''} >> std::noskipws >> c >> Demand{'\''})
  {
    value.link_ = c;
  }
  return in;
}

template<>
std::ostream& andriuschin::operator<<< andriuschin::RationalLiteral::value_type,
      andriuschin::RationalLiteral::id >(std::ostream& out, const RationalLiteral&& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << "(:N " << value.link_.first << ":D " << value.link_.second << ":)";
}
template<>
std::istream& andriuschin::operator>>< andriuschin::RationalLiteral::value_type,
      andriuschin::RationalLiteral::id >(std::istream& in, RationalLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  FormatGuard guard(in);
  unsigned long long denominator = 0;
  long long numerator = 0;

  in >> Demand{'('} >> std::noskipws >> Demand{':'};
  in >> Demand{'N'} >> Demand{' '} >> numerator;
  in >> Demand{':'} >> Demand{'D'} >> Demand{' '};
  if (!in || in.peek() == '-')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> denominator >> Demand{':'};
  in >> std::noskipws >> Demand{')'};
  if (!in || denominator == 0)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  value.link_ = {numerator, denominator};
  return in;
}

template<>
std::ostream& andriuschin::operator<<< andriuschin::StringLiteral::value_type,
      andriuschin::StringLiteral::id >(std::ostream& out, const StringLiteral&& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << '"' << value.link_ << '"';
}
template<>
std::istream& andriuschin::operator>>< andriuschin::StringLiteral::value_type,
      andriuschin::StringLiteral::id >(std::istream& in, StringLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string temp;

  if (std::getline(in >> Demand{'"'}, temp, '"'))
  {
    value.link_ = std::move(temp);
  }
  return in;
}
