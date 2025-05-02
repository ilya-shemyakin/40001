#ifndef  DATA_STRUCT_H
#define  DATA_STRUCT_H

#include <iomanip>
#include <iostream>
#include <string>

namespace nspace
{
  struct DataStruct
  {
    double key1;
    std::pair<long long, unsigned long long> key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct DoubleIO
  {
    double& ref;
  };

  struct RatLspIO
  {
    std::pair<long long, unsigned long long>& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct LabelIO
  {
    std::string exp;
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
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, RatLspIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

  bool compareDataStruct(const DataStruct& a, const DataStruct& b);

}

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

  std::istream& operator>>(std::istream& in, RatLspIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    std::streampos startPos = in.tellg();

    if (!(in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' }))
    {
      in.seekg(startPos);
      return in;
    }

    if (!(in >> DelimiterIO{ 'N' }))
    {
      in.seekg(startPos);
      return in;
    }

    long long numerator;
    if (!(in >> numerator))
    {
      in.seekg(startPos);
      return in;
    }

    if (!(in >> DelimiterIO{ ':' } >> DelimiterIO{ 'D' }))
    {
      in.seekg(startPos);
      return in;
    }

    unsigned long long denominator;
    if (!(in >> denominator))
    {
      in.seekg(startPos);
      return in;
    }

    if (denominator == 0)
    {
      in.seekg(startPos);
      in.setstate(std::ios::failbit);
      return in;
    }

    if (!(in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' }))
    {
      in.seekg(startPos);
      return in;
    }

    dest.ref = { numerator,denominator };
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
        in >> RatLspIO{ input.key2 } >> DelimiterIO{ ':' };
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
    out << "key2 (:N " << dest.key2.first << ":D " << dest.key2.second << ":):";
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
      return a.key2.first * b.key2.second < b.key2.first * a.key2.second;
    }

    return a.key3.length() < b.key3.length();
  }
}
#endif

