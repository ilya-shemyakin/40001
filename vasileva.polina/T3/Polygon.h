#ifndef POLYGON_H
#define POLYGON_H
#include <vector>

struct Point
{
    int x, y;
};

struct Polygon
{
    std::vector<Point> points;
};
using polys = std::vector<Polygon>;

#endif
