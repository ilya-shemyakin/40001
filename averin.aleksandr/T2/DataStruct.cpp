#include "DataStruct.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>

namespace averin
{
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

        std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
        {
            std::istream::sentry sentry(in);
            if (!sentry)
            {
                return in;
            }
            char c = ' ';
            in >> c;
            if (in && (c != dest.exp))
            {
                in.setstate(std::ios::failbit);
            }
            return in;
        }

    std::istream& operator>>(std::istream& in, ULLBinIO&& dest) 
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


    std::istream& operator>>(std::istream& in, ULLLitIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        iofmtguard guard(in);
        unsigned long long temp = 0;
        char ullTag[4] = { 0 };

        if (!std::isdigit(in.peek()) || !(in >> temp))
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        in >> std::setw(4) >> ullTag;
        if ((strcmp(ullTag, "ull") != 0 && strcmp(ullTag, "ULL") != 0))
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        dest.ref = temp;
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string data = "";
        if ((in >> data) && (data != dest.exp))
        {
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
		{
			using sep = DelimiterIO;
			using ull_lit = ULLLitIO;
			using ull_bin = ULLBinIO;
			using str = StringIO;

			in >> sep{ '(' } >> sep{ ':' };

			for (size_t i = 0; i < 3; ++i)
			{
				std::string label;
				in >> label;
				if (label == "key1")
				{
					in >> ull_lit{ input.key1 } >> sep{ ':' };
				}
				else if (label == "key2")
				{
					in >> ull_bin{ input.key2 } >> sep{ ':' };
				}
				else if (label == "key3")
				{
					in >> str{ input.key3 } >> sep{ ':' };
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

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        iofmtguard fmtguard(out);
        out << "(:key1 " << src.key1 << "ull:key2 0" << std::oct << src.key2 << std::dec << ":key3 \"" << src.key3 << "\":)";
        return out;
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

}
