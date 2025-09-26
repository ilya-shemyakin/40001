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
    if (!sentry) return in;

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
    if (!sentry) return in;
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, CharIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char quote, ch, endquote;
    in >> quote >> ch >> endquote;

    if (quote == '\'' && endquote == '\'')
    {
      dest.ref = ch;
      return in;
    }

    in.setstate(std::ios::failbit);
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    double value;
    if (in >> value)
    {
      char next = in.peek();
      if (next == 'd' || next == 'D')
      {
        in.ignore(1);
      }
      dest.ref = value;
      return in;
    }

    in.setstate(std::ios::failbit);
    return in;
  }

  std::istream& operator>>(std::istream& in, UllIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char first = in.peek();

    if (first == '0')
    {
      in.get();
      char second = in.peek();

      if (second == 'x' || second == 'X')
      {
        in.get();
        in >> std::hex >> dest.ref;
      }
      else if (second == 'b' || second == 'B')
      {
        in.get();
        in >> dest.ref;
      }
      else
      {
        in >> std::oct >> dest.ref;
      }
    }
    else
    {
      in >> dest.ref;

      char suffix1 = in.peek();
      if (suffix1 == 'u' || suffix1 == 'U')
      {
        in.get();
        char suffix2 = in.peek();
        if (suffix2 == 'l' || suffix2 == 'L')
        {
          in.get();
        }
      }
      else if (suffix1 == 'l' || suffix1 == 'L')
      {
        in.get();
        char suffix2 = in.peek();
        if (suffix2 == 'l' || suffix2 == 'L')
        {
          in.get();
        }
      }
    }

    if (!in) in.setstate(std::ios::failbit);
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct input;

    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

    int fieldsRead = 0;
    std::string fieldName;

    while (in >> fieldName && fieldName != ")")
    {
      if (fieldName == "key1")
      {
        in >> DoubleIO{ input.key1 } >> DelimiterIO{ ':' };
        fieldsRead |= 1;
      }
      else if (fieldName == "key2")
      {
        in >> UllIO{ input.key2 } >> DelimiterIO{ ':' };
        fieldsRead |= 2;
      }
      else if (fieldName == "key3")
      {
        in >> StringIO{ input.key3 } >> DelimiterIO{ ':' };
        fieldsRead |= 4;
      }
      else
      {
        std::string value;
        in >> value >> DelimiterIO{ ':' };
      }
    }

    if (fieldsRead == 7)
    {
      dest = input;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) return out;

    iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1);

    out << "(:key1 " << dest.key1 << "d:";
    out << "key2 0x" << std::hex << std::uppercase << dest.key2 << ":";
    out << "key3 \"" << dest.key3 << "\":)";

    return out;
  }

  iofmtguard::iofmtguard(std::basic_ios< char >& s) :
    s_(s), width_(s.width()), fill_(s.fill()),
    precision_(s.precision()), fmt_(s.flags())
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
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
  }
}

