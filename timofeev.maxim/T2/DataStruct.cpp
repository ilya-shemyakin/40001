#include "DataStruct.hpp"

namespace timofeev {
  bool dataStructCompare(const DataStruct& a, const DataStruct& b)
  {
    if (a.key1 != b.key1)
    {
      return a.key1 < b.key1;
    }

    if (a.key2 != b.key2)
    {
      return a.key2 < b.key2;
    }

    return (a.key3).length() < (b.key3).length();
  }
  std::string beautyDouble(double check)
  {
    std::ostringstream oss;
    int a = 0;
    while (!(abs(check) < 10 && abs(check) >= 1))
    {
      if (check >= 10)
      {
        a++;
        check /= 10;
      }
      else if (check < 1)
      {
        a--;
        check *= 10;
      }
    }
    if (std::abs(check) >= 1.01)
    {
      oss << std::fixed << std::setprecision(2) << check << std::showpos << "e" << a;
    }
    else
    {
      oss << std::fixed << std::setprecision(1) << check << std::showpos << "e" << a;
    }
    std::string toWrite = oss.str();
    return toWrite;
  }

  bool has_e(std::string& check)
  {
    for (size_t i = 0; i < check.size(); i++)
    {
      if (check[i] == 'e' || check[i] == 'E')
      {
        return true;
      }
    }
    return false;
  }

  std::string utos(unsigned long long key)
  {
    if (key == 0)
    {
      return "0b0";
    }
    else
    {
      std::string binaryPart = "0b0";
      while (key != 0)
      {
        binaryPart.insert(3, (key % 2 == 1) ? "1" : "0");
        key /= 2;
      }
      return binaryPart;
    }
  }
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

  std::istream& operator>>(std::istream& in, DoubleIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    std::string check;
    getline(in, check, ':');
    in.unget();
    if (check[0] >= '1' && check[0] <= '9' && check[1] == '.' && has_e(check))
    {
      double toReturn = stod(check);
      dest.ref = toReturn;
    }
    else
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

  std::istream& operator>>(std::istream& in, UllBinIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    std::string toCheck = "";
    getline(in, toCheck, ':');
    in.unget();
    if (toCheck[0] != '0' || (toCheck[1] != 'b' && toCheck[1] != 'B') || toCheck == "")
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    for (size_t i = 2; i < toCheck.length(); i++)
    {
      if (toCheck[i] != '0' && toCheck[i] != '1')
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    unsigned long long toWrite = stoull(toCheck.substr(2), nullptr, 2);
    dest.ref = toWrite;
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
    {
      using sep = DelimiterIO;
      using dbl = DoubleIO;
      using str = StringIO;
      using ull = UllBinIO;
      in >> sep{ '(' } >> sep{ ':' };
      for (size_t i = 0; i < 3; i++)
      {
        std::string label = "";
        in >> label;
        if (label == "key1")
        {
          in >> dbl{ input.key1 };
          in >> sep{ ':' };
        }
        else if (label == "key2")
        {
          in >> ull{ input.key2 };
          in >> sep{ ':' };
        }
        else if (label == "key3")
        {
          in >> str{ input.key3 };
          in >> sep{ ':' };
        }
        else
        {
          in.setstate(std::ios::failbit);
          return in;
        }
      }
      in >> sep{ ')' };
    }
    if (in)
    {
      dest = input;
    }
    return in;
  }

  std::ostream& operator<< (std::ostream& out, const DataStruct& data)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    iofmtguard fmtguard(out);
    out << "(";
    out << ":" << "key1" << " " << beautyDouble(data.key1);
    out << ":" << "key2" << " " << utos(data.key2);
    out << ":" << "key3" << " " << '"' << data.key3 << '"';
    out << ":";
    out << ")";
    return out;
  }

  iofmtguard::iofmtguard(std::basic_ios< char >& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  iofmtguard::~iofmtguard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}
