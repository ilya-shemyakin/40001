#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iosfwd>
#include <vector>

namespace andriuschin
{
  struct Point
  {
    int x, y;
  };
  std::istream& operator>>(std::istream& in, Point& point);
  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream& operator>>(std::istream& in, Polygon& polygon);

  struct GetArea
  {
    double operator()(const andriuschin::Polygon& polygon);
  };
  struct IsInside
  {
    bool operator()(const Polygon& poly, const Point& point);
  };
  struct GetIntersections
  {
    bool operator()(const Polygon& lhs, const Polygon& rhs);
  };

  namespace details
  {
    struct GetTriangulatedArea
    {
      Point buf[2];
      size_t size = 0;
      double operator()(const Point& point);
    };
    struct IsRightOnTheSameHeight
    {
      static constexpr size_t ON_SIDE = 10;
      Point prev;
      const Point& point;
      size_t operator()(const Point& next);
    };
  }
}

#endif
