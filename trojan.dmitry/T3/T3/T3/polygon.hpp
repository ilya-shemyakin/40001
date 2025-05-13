#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iostream>

struct Point
{
    int x, y;
};
struct Polygon
{
    std::vector< Point > points;
};
struct Delimiter
{
    char expected;
};
struct Int
{
    int& reference;
};

std::istream& operator>>(std::istream& stream, Delimiter&& value);
std::istream& operator>>(std::istream& stream, Int&& value);
std::istream& operator>>(std::istream& stream, Polygon& value);

#endif
