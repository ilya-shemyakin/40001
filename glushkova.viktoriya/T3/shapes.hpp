#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <iostream>
#include <vector>

namespace shapes
{
    struct Point
    {
        int x, y;
        bool operator==(const Point& other) const;
        bool operator!=(const Point& other) const;
        bool operator<(const Point& other) const;
    };

    struct Polygon
    {
        std::vector<Point> points;
        bool operator==(const Polygon& other) const;
        bool operator!=(const Polygon& other) const;
    };

    std::istream& operator>>(std::istream& in, Point& point);
    std::istream& operator>>(std::istream& in, Polygon& poly);
    double calcArea(const Polygon& poly);
    bool arePolygonsSame(const Polygon& poly1, const Polygon& poly2);
}
#endif
