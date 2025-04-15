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
  bool has_e(std::string check)
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

  bool parsingString(std::istream& in, std::string& key3)
  {
    std::string toReturn = "";
    char c = ' ';
    getline(in >> c, toReturn, '"');
    if (c == '"')
    {
      key3 = toReturn;
      char preend = ' ';
      in >> preend;
      return true;
    }
    return false;
  }

  bool parsingDouble(std::istream& in, double& key1)
  {
    try
    {
      std::string check;
      getline(in, check, ':');
      check.erase(0, 1);
      double toReturn = stod(check);
      if (check[0] >= '1' && check[0] <= '9' && check[1] == '.' && has_e(check))
      {
        key1 = toReturn;
        return true;
      }
      else
      {
        throw std::invalid_argument("");
      }
    }
    catch (...)
    {
      return false;
    }
  }

  bool parsingUll(std::istream& in, unsigned long long& key2)
  {
    try
    {
      std::string toCheck = "";
      getline(in, toCheck, ':');
      toCheck.erase(0, 1);
      unsigned long long toWrite = stoull(toCheck.substr(2), nullptr, 2);
      if (toCheck[0] != '0' || (toCheck[1] != 'b' && toCheck[1] != 'B'))
      {
        throw std::invalid_argument("");
      }
      for (size_t i = 2; i < toCheck.length(); i++)
      {
        if (toCheck[i] != '0' && toCheck[i] != '1')
        {
          return false;
        }
      }
      key2 = toWrite;
      return true;
    }
    catch (...)
    {
      return false;
    }
  }
  std::istream& operator >> (std::istream& in, DataStruct& data)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    DataStruct input;
    char c = ' ';
    char b = ' ';
    in >> c;
    in >> b;
    if (c != '(' && b != ':')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    for (size_t i = 0; i < 3; i++)
    {
      std::string key = "";
      in >> key;
      if (key == "key1")
      {
        if (!parsingDouble(in, input.key1))
        {
          in.setstate(std::ios::failbit);
          return in;
        }
      }
      else if (key == "key2")
      {
        if (!parsingUll(in, input.key2))
        {
          in.setstate(std::ios::failbit);
          return in;
        }
      }
      else if (key == "key3")
      {
        if (!parsingString(in, input.key3))
        {
          in.setstate(std::ios::failbit);
          return in;
        }
      }
    }
    char end = ' ';
    in >> end;
    if (end != ')')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (in)
    {
      data = input;
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
    out << "(";
    out << ":" << "key1" << " " << beautyDouble(data.key1);
    out << ":" << "key2" << " " << utos(data.key2);
    out << ":" << "key3" << " " << '"' << data.key3 << '"';
    out << ":";
    out << ")";
    return out;
  }
}
