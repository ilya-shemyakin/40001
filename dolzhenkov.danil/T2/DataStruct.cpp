#include "DataStruct.h"
#include <regex>

namespace dolzhenkov {
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
        std::stringstream ss;
        ss << std::scientific << check;
        std::string str = ss.str();
        size_t i = str.find('e');
        while (str[i - 1] == '0' && str[i - 2] != '.')
        {
            str.erase(i - 1, 1);
            i = str.find('e');
        }
        while (str[i + 2] == '0')
        {
            str.erase(i + 2, 1);
            i = str.find('e');
        }
        return str;
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

    bool isDoubleSCI(const std::string& str)
    {
        std::regex pattern("^[-+]?[0-9]*\\.?[0-9]+([eE][-+][0-9]+)$");
        return std::regex_match(str, pattern);
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string str;
        std::getline(in, str, ':');
        in.putback(':');
        if (isDoubleSCI(str))
        {
            std::stringstream ss(str);
            ss >> dest.ref;
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

    std::istream& operator>>(std::istream& in, SllLitIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        std::string toCheck;
        getline(in, toCheck, ':');
        in.putback(':');

        bool endsWithLL = (toCheck.size() >= 3)
            && ((toCheck.substr(toCheck.size() - 2) == "ll" || toCheck.substr(toCheck.size() - 2) == "LL"));

        if (!endsWithLL) {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string numberPart = toCheck.substr(0, toCheck.size() - 2);
        std::istringstream iss(numberPart);
        long long value;
        iss >> value;

        if (iss.fail() || !iss.eof()) {
            in.setstate(std::ios::failbit);
            return in;
        }

        dest.ref = value;
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
            using sll = SllLitIO;
            bool done[3] = { false, false, false };

            in >> sep{ '(' } >> sep{ ':' };
            for (size_t i = 0; i < 3; i++)
            {
                std::string label = "";
                in >> label;
                if (label == "key1")
                {
                    in >> dbl{ input.key1 } >> sep{ ':' };
                    done[0] = true;
                }
                else if (label == "key2")
                {
                    in >> sll{ input.key2 } >> sep{ ':' };
                    done[1] = true;
                }
                else if (label == "key3")
                {
                    in >> str{ input.key3 } >> sep{ ':' };
                    done[2] = true;
                }
                else
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }

            }
            in >> sep{ ')' };
            if (!done[0] || !done[1] || !done[2])
            {
                in.setstate(std::ios::failbit);
                return in;
            }

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
        out << ":" << "key2" << " " << data.key2 << "ll";
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
