#include "DataStruct.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cctype>

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
    in >> dest.ref;
    if (in && std::tolower(in.peek()) == 'd')
    {
        in.ignore();
    }
    return in;
}

std::istream& operator>>(std::istream& in, LongLongIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    in >> dest.ref;

    if (in)
    {
        char next = in.peek();
        if (next == 'l' || next == 'L')
        {
            in.ignore();
            if (in.peek() == 'l' || in.peek() == 'L')
            {
                in.ignore();
            }
        }
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

    char quote;
    in >> std::ws >> quote;
    if (quote != '"')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string value;
    std::getline(in, value, '"');
    dest.ref = value;

    return in;
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
    if (in && (data != dest.exp))
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
        using dbl = DoubleIO;
        using ll = LongLongIO;
        using str = StringIO;

        in >> sep{ '(' } >> sep{ ':' };

        int fieldsRead = 0;
        while (fieldsRead < 3 && in)
        {
            std::string fieldName;
            in >> fieldName;

            if (fieldName == "key1")
            {
                in >> dbl{ input.key1 };
                fieldsRead++;
            }
            else if (fieldName == "key2")
            {
                in >> ll{ input.key2 };
                fieldsRead++;
            }
            else if (fieldName == "key3")
            {
                in >> str{ input.key3 };
                fieldsRead++;
            }
            else
            {
                in.setstate(std::ios::failbit);
                break;
            }

            if (fieldsRead < 3 && in)
            {
                in >> sep{ ':' };
            }
        }

        if (in)
        {
            in >> sep{ ':' } >> sep{ ')' };
        }
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
    out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
    out << ":key2 " << src.key2 << "ll";
    out << ":key3 \"" << src.key3 << "\":)";

    return out;
}

bool compareDataStructs(const DataStruct& a, const DataStruct& b)
{
    const double eps = 1e-9;
    if (std::abs(a.key1 - b.key1) > eps)
    {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2)
    {
        return a.key2 < b.key2;
    }
    return a.key3.length() < b.key3.length();
}

iofmtguard::iofmtguard(std::basic_ios<char>& s) :
    s_(s),
    precision_(s.precision()),
    fmt_(s.flags())
{
}

iofmtguard::~iofmtguard()
{
    s_.precision(precision_);
    s_.flags(fmt_);
}
