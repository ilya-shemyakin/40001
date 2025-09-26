#include "DataStruct.hpp"

namespace nspace {

bool dataStructCompare(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (std::abs(a.key2) != std::abs(b.key2)) return std::abs(a.key2) < std::abs(b.key2);
    return a.key3.length() < b.key3.length();
}

std::string formatDouble(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << value;
    return oss.str();
}

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    char c;
    in >> c;
    if (c != dest.exp) in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, SllIO&& dest)
{
    std::string tmp;
    getline(in, tmp, ':');
    in.unget();
    try { dest.ref = std::stoll(tmp); } 
    catch (...) { in.setstate(std::ios::failbit); }
    return in;
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest)
{
    char c;
    in >> c;
    if (c != '#') { in.setstate(std::ios::failbit); return in; }
    in >> c;
    if (c != 'c') { in.setstate(std::ios::failbit); return in; }
    in >> DelimiterIO{'('};
    double re, im;
    in >> re >> im;
    dest.ref = std::complex<double>(re, im);
    in >> DelimiterIO{')'};
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    DataStruct tmp;
    in >> DelimiterIO{'('} >> DelimiterIO{':'};
    for (int i = 0; i < 3; ++i) {
        std::string label;
        in >> label;
        if (label == "key1") in >> SllIO{tmp.key1} >> DelimiterIO{':'};
        else if (label == "key2") in >> ComplexIO{tmp.key2} >> DelimiterIO{':'};
        else if (label == "key3") in >> StringIO{tmp.key3} >> DelimiterIO{':'};
        else { in.setstate(std::ios::failbit); return in; }
    }
    in >> DelimiterIO{')'};
    if (in) dest = tmp;
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
    iofmtguard guard(out);
    out << "(:";
    out << "key1 " << data.key1 << ":";
    out << "key2 #c(" << formatDouble(data.key2.real()) 
        << " " << formatDouble(data.key2.imag()) << "):";
    out << "key3 \"" << data.key3 << "\":)";
    return out;
}

iofmtguard::iofmtguard(std::basic_ios<char>& s)
    : s_(s), width_(s.width()), fill_(s.fill()), precision_(s.precision()), fmt_(s.flags()) {}

iofmtguard::~iofmtguard()
{
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}

}
