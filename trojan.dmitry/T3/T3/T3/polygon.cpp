#include "polygon.hpp"

std::istream& operator>>(std::istream& stream, Delimiter&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry)
    {
        return stream;
    }
    char c = '\0';
    stream >> c;
    if (stream && c != value.expected)
    {
        stream.setstate(std::ios::failbit);
    }
    return stream;
}
std::istream& operator>>(std::istream& stream, Int&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry)
    {
        return stream;
    }
    return stream >> value.reference;
}
