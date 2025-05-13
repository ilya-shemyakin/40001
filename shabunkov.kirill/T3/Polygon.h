#ifndef POLYGON_H
#define POLYGON_H
#include <vector>

struct Point
{
  int x;
  int y;
};

struct Polygon
{
  std::vector<Point> points;
};

struct PointInFrameChecker
{
  int minX_, minY_, maxX_, maxY_;
  PointInFrameChecker(int minX, int minY, int maxX, int maxY)
    : minX_(minX), minY_(minY), maxX_(maxX), maxY_(maxY) {
  }

  bool operator()(const Point& p) const
  {
    return p.x >= minX_ && p.x <= maxX_ && p.y >= minY_ && p.y <= maxY_;
  }
};

struct AngleChecker
{
  const std::vector<Point>& p;
  size_t i = 0;

  bool operator()(const Point&)
  {
    const Point& prev = p[(i + p.size() - 1) % p.size()];
    const Point& curr = p[i];
    const Point& next = p[(i + 1) % p.size()];
    i++;

    return (prev.x - curr.x) * (next.x - curr.x)
      + (prev.y - curr.y) * (next.y - curr.y) == 0;
  }
};

#endif

