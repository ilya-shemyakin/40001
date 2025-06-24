#include "polygon.hpp"
#include "functions.hpp"
#include "functors.hpp"
#include <utility>
#include <iomanip>
#include <iterator>
#include "stream_guard.hpp"

std::istream& operator>>(std::istream& stream, Delimiter&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    char c = '\0';
    if ((stream >> c) && (c != value.expected)) {
        stream.setstate(std::ios::failbit);
    }
    return stream;
}
std::istream& operator>>(std::istream& stream, Point& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    int x, y;
    if (stream >> Delimiter{ '(' } >> x >> Delimiter{';'} >> y >> Delimiter{ ')' }) {
        value.x = x;
        value.y = y;
    }
    return stream;
}
std::istream& operator>>(std::istream& stream, Polygon& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    trojan::StreamGuard guard(stream);
    std::vector< Point > temp;
    size_t count = 0;
    if ((stream.peek() == '-') || (!stream >> count) || (count < 3)) {
        stream.setstate(std::ios::failbit);
    }
    else {
        std::copy_n(std::istream_iterator< Point >(stream), count, std::back_inserter(temp));
        if (temp.size() == count) {
            value.points = std::move(temp);
        }
        else {
            stream.setstate(std::ios::failbit);
        }
    }
    return stream;
}
