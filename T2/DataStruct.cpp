#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include "DataStruct.h"

namespace nspace
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, LabelIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    std::string data;
    in >> data;

    if ((data != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    char suff = '\0';

    if (in >> dest.ref >> suff)
    {
      if (suff == 'd' || suff == 'D')
      {
        return in;
      }
    }

    in.setstate(std::ios::failbit);
    return in;
  }

  std::istream& operator>>(std::istream& in, UllHexIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char c1, c2;
    if (in >> c1 >> c2 && !(c1 == '0' && (c2 == 'x' || c2 == 'X'))) {
      in.setstate(std::ios::failbit);
      return in;
    }

    in >> std::hex >> dest.ref;
    if (!in) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    DataStruct input;
    std::string label;
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
    while (in >> label)
    {
      if (label == "key1")
      {
        in >> DoubleIO{ input.key1 } >> DelimiterIO{ ':' };
      }
      else if (label == "key2")
      {
        in >> UllHexIO{ input.key2 } >> DelimiterIO{ ':' };
      }
      else if (label == "key3")
      {
        in >> StringIO{ input.key3 } >> DelimiterIO{ ':' };
      }
      else if (label == ")")
      {
        break;
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }
    }

    if (in)
    {
      dest = input;
    }

    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }

    iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1);

    out << "(:";
    out << "key1 " << dest.key1 << "d:";
    out << "key2 0x" << std::hex << std::uppercase << dest.key2 << ":";
    out << "key3 \"" << dest.key3 << "\":" << ")";

    return out;
  }

  iofmtguard::iofmtguard(std::basic_ios< char >& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {
  }

  iofmtguard::~iofmtguard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }

  bool compareDataStruct(const DataStruct& a, const DataStruct& b)
  {
    if (a.key1 != b.key1)
    {
      return a.key1 < b.key1;
    }

    if (a.key2 != b.key2)
    {
      return a.key2 < b.key2;
    }

    return a.key3.length() < b.key3.length();
  }
}

