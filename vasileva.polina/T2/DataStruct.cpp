#include "DataStruct.h"

#include <iostream>
#include <sstream>

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

    std::istream& operator>>(std::istream& in, ULLOctIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string str;
        char c;

        while (in.get(c)) {
            if (c == ':' || isspace(c)) {
                in.unget();
                break;
            }
            str.push_back(c);
        }
        try
        {
            dest.ref = std::stoull(str, nullptr, 8);
        }
        catch (...)
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, ULLLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        std::string numStr;
        char c;

        while (in.get(c)) {
            if (c == ':' || isspace(c)) {
                in.unget();
                break;
            }
            numStr.push_back(c);
        }

        size_t pos = numStr.find("ull");
        if (pos == std::string::npos) {
            pos = numStr.find("ULL");
            if (pos == std::string::npos) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        try {
            dest.ref = std::stoull(numStr.substr(0, pos));
        }
        catch (...) {
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
        std::string data = "";
        if ((in >> data) && (data != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        DataStruct temp;
        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

        std::string label;
        while (in >> label) {
            if (label == "key1") {
                in >> ULLLitIO{ temp.key1 } >> DelimiterIO{ ':' };
            }
            else if (label == "key2") {
                in >> ULLOctIO{ temp.key2 } >> DelimiterIO{ ':' };
            }
            else if (label == "key3") {
                in >> StringIO{ temp.key3 } >> DelimiterIO{ ':' };
            }
            else if (label == ")") {
                break;
            }
            else {
                in.setstate(std::ios::failbit);
                break;
            }
        }

        if (in) {
            dest = temp;
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

    bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }
        if (a.key2 != b.key2) {
            return a.key2 < b.key2;
        }
        return a.key3.length() < b.key3.length();
    }
}
