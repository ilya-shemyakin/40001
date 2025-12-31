#include "dataStruct.hpp"
#include <vector>
#include <iomanip>
#include <cmath>
#include <complex>
#include "scopeGuard.hpp"

namespace
{
    struct DelimiterIO
    {
        char expected;
    };

    struct OctUllIO
    {
        unsigned long long& value;
    };

    struct ComplexNumIO
    {
        std::complex<double>& value;
    };

    struct StringValueIO
    {
        std::string& value;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        char c = '\0';
        in >> c;
        if (in && (c != dest.expected))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, OctUllIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        in >> DelimiterIO{ '0' } >> std::oct >> dest.value;

        if (in)
        {
            unsigned long long temp = dest.value;
            while (temp > 0)
            {
                if (temp % 10 >= 8)
                {
                    in.setstate(std::ios::failbit);
                    break;
                }
                temp /= 10;
            }
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, ComplexNumIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        double real = 0.0, imag = 0.0;
        in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };
        in >> real >> imag;
        in >> DelimiterIO{ ')' };

        if (in)
        {
            dest.value = std::complex<double>(real, imag);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringValueIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        return std::getline(in >> DelimiterIO{ '"' }, dest.value, '"');
    }
}

std::istream& vladimirova::operator>>(std::istream& in, vladimirova::DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct temp;
    {
        std::vector<std::string> fields(3);
        in >> DelimiterIO{ '(' };

        for (size_t i = 0; i < 3; ++i)
        {
            in >> DelimiterIO{ ':' } >> fields[i];

            if (fields[i] == "key1")
            {
                in >> OctUllIO{ temp.key1 };
            }
            else if (fields[i] == "key2")
            {
                in >> ComplexNumIO{ temp.key2 };
            }
            else if (fields[i] == "key3")
            {
                in >> StringValueIO{ temp.key3 };
            }
            else
            {
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
    }

    if (in)
    {
        dest = temp;
    }
    return in;
}

std::ostream& vladimirova::operator<<(std::ostream& out, const vladimirova::DataStruct& dest)
{
    std::ostream::sentry sentry(out);
    if (!sentry) return out;

    iofmtguard guard(out);
    out << "(:key1 0" << std::oct << dest.key1;
    out << ":key2 #c(" << std::fixed << std::setprecision(1);
    out << dest.key2.real() << " " << dest.key2.imag() << ")";
    out << ":key3 \"" << dest.key3 << "\":)";

    return out;
}

bool vladimirova::DataStruct::operator<(const DataStruct& other) const
{
    if (key1 != other.key1)
    {
        return key1 < other.key1;
    }
    else if (std::abs(key2) != std::abs(other.key2))
    {
        return std::abs(key2) < std::abs(other.key2);
    }
    else
    {
        return key3.length() < other.key3.length();
    }
}

