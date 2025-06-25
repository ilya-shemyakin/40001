#ifndef POLYGON_H
#define POLYGON_H
#include <vector>

struct Point
{
  int x;
  int y;

  bool operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Point& other) const
  {
    return !(*this == other);
  }
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

  bool operator()(const Point& curr) const
  {
    auto it = std::find(p.begin(), p.end(), curr);
    if (it == p.end()) return false;

    size_t i = std::distance(p.begin(), it);
    const Point& prev = p[(i + p.size() - 1) % p.size()];
    const Point& next = p[(i + 1) % p.size()];

    return (prev.x - curr.x) * (next.x - curr.x)
      + (prev.y - curr.y) * (next.y - curr.y) == 0;
  }
};

#endif

