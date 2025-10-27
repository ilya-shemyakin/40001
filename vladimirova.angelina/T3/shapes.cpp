#include "shapes.hpp"
#include <iterator>
#include <algorithm>
#include "delimiter.hpp"

std::istream& vladimirova::operator>>(std::istream& in, Point& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    in >> DelimiterIO{ '(' };
    in >> dest.x >> DelimiterIO{ ';' };
    in >> dest.y >> DelimiterIO{ ')' };
    return in;
}

std::istream& vladimirova::operator>>(std::istream& in, Polygon& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    size_t vertexCount = 0;
    if (!(in >> vertexCount) || vertexCount < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::vector<Point> tempPoints(vertexCount);
    std::copy_n(std::istream_iterator<Point>(in), vertexCount, tempPoints.begin());
    if (!in)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    dest.points = std::move(tempPoints);
    return in;
}

bool vladimirova::Point::operator==(const Point& rhs) const
{
    return x == rhs.x && y == rhs.y;
}

vladimirova::Point vladimirova::Point::operator-(const Point& rhs) const
{
    return { x - rhs.x, y - rhs.y };
}

bool vladimirova::Polygon::operator==(const Polygon& rhs) const
{
    return points == rhs.points;
}
