#include "input_parser.hpp"

std::istream& shapes::operator>>(std::istream& in, Delimiter&& exp)
{
    std::istream::sentry guard(in);
    if (!guard)
    {
        return in;
    }
    char c = 0;
    in >> c;
    if (c != exp.expected)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}
