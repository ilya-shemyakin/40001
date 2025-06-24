#include <iomanip>
#include "DataStruct.h"

namespace alymov
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

    std::istream& operator>>(std::istream& in, ULLHexIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        char c1 = in.get();
        char c2 = in.get();
        if (!in || (c1 != '0') || (c2 != 'x' && c2 != 'X'))
        {
            in.setstate(std::ios::failbit);

            return in;
        }
        StreamGuard guard(in);
        in >> std::hex >> dest.ref;
        return in;
    }

    std::istream& operator>>(std::istream& in, ComplexIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        double real = 0.0;
        double imag = 0.0;
        in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };
        in >> real >> imag;
        in >> DelimiterIO{ ')' };
        dest.ref = std::complex<double>(real, imag);
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

    bool isValidKey(const std::string& key)
    {
        return key == "key1" || key == "key2" || key == "key3";
    }

    bool checkAndSkipSpace(std::istream& in)
    {
        if (in.peek() == ' ')
        {
            in.setstate(std::ios::failbit);
            return false;
        }
        return true;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        DataStruct input;
        using sep = DelimiterIO;
        using ullHex = ULLHexIO;
        using cmp = ComplexIO;
        using str = StringIO;

        std::string key = "";
        in >> sep{ '(' };

        if (!checkAndSkipSpace(in)) return in;
        in >> sep{ ':' };

        for (int i = 0; i < 3; i++)
        {
            if (!checkAndSkipSpace(in)) return in;
            in >> key;
            if (!isValidKey(key))
            {
                in.setstate(std::ios::failbit);
                return in;
            }

            if (key == "key1")
            {
                in >> ullHex{ input.key1 };
            }
            else if (key == "key2")
            {
                in >> cmp{ input.key2 };
            }
            else if (key == "key3")
            {
                in >> str{ input.key3 };
            }

            if (!checkAndSkipSpace(in)) return in;
            in >> sep{ ':' };
        }

        if (!checkAndSkipSpace(in)) return in;
        in >> sep{ ')' };

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
        StreamGuard guard(out);
        out << "(:key1 0x" << std::uppercase << std::hex << src.key1
            << ":key2 " << std::fixed << std::setprecision(1)
            << "#c(" << src.key2.real() << " " << src.key2.imag() << ")"
            << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    bool compareDataStruct(const DataStruct& a, const DataStruct& b)
    {
        const double EPSILON = 1e-9;
        if (a.key1 != b.key1)
        {
            return a.key1 < b.key1;
        }
        double aMagnitude = std::norm(a.key2);
        double bMagnitude = std::norm(b.key2);
        if (std::abs(aMagnitude - bMagnitude) > EPSILON)
        {
            return aMagnitude < bMagnitude;
        }
        return a.key3.length() < b.key3.length();
    }
}
