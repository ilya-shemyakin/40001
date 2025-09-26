#include "polygon.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <functional>
#include <numeric>
#include <limits>

#include <demand.hpp>
#include <format_guard.hpp>

std::istream& andriuschin::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  FormatGuard guard(in);
  int x, y;
  if (in >> Demand{'('} >> x >> Demand{';'} >> y >> Demand{')'})
  {
    point.x = x;
    point.y = y;
  }
  return in;
}
std::istream& andriuschin::operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  FormatGuard guard(in);
  size_t count;
  if ((in.peek() != '-') && (in >> count) && (count >= 3))
  {
    std::vector< Point > temp;
    std::copy_n(std::istream_iterator< Point >(in), count, std::back_inserter(temp));
    if (temp.size() == count)
    {
      polygon.points = std::move(temp);
      return in;
    }
  }
  in.setstate(std::ios::failbit);
  return in;
}

double andriuschin::details::GetTriangulatedArea::operator()(const Point& point)
{
  if (size < 2)
  {
    buf[size++] = point;
    return 0;
  }

  using namespace std::placeholders;
  static auto getX = std::bind(std::mem_fn(&Point::x), _1);
  static auto getY = std::bind(std::mem_fn(&Point::y), _1);
  static auto getXDist = std::bind(std::minus<>{}, std::bind(getX, _1), std::bind(getX, _2));
  static auto getYDist = std::bind(std::minus<>{}, std::bind(getY, _1), std::bind(getY, _2));
  static auto sqr = std::bind(std::multiplies<>{}, _1, _1);
  static auto getDistSqr = std::bind(std::plus<>{}, std::bind(sqr, getXDist), std::bind(sqr, getYDist));
  static double(*sqrt)(double) = std::sqrt;
  static auto getDist = std::bind(sqrt, getDistSqr);

  Point a = buf[1];
  buf[1] = point;
  double sides[3] = {getDist(a, buf[0]), getDist(buf[0], point), getDist(point, a)};
  double semiPerimetr = (sides[0] + sides[1] + sides[2]) / 2;
  return sqrt(semiPerimetr * (semiPerimetr - sides[0]) * (semiPerimetr - sides[1]) * (semiPerimetr - sides[2]));
}
double andriuschin::GetArea::operator()(const Polygon& polygon)
{
  using namespace std::placeholders;
  auto adder = std::bind(std::plus<>{}, _1, std::bind(details::GetTriangulatedArea{}, _2));
  return std::accumulate(polygon.points.begin(), polygon.points.end(), 0.0, adder);
}

size_t andriuschin::details::IsRightOnTheSameHeight::operator()(const Point& next)
{
  Point current = prev;
  prev = next;
  if (((current.y >= point.y) && (next.y <= point.y)) || ((current.y <= point.y) && (next.y >= point.y)))
  {
    double height = next.y - current.y;
    double width = next.x - current.x;
    double intersectX = point.x;
    if (height != 0)
    {
      intersectX = current.x + (point.y - current.y) / height * width;
    }
    if (point.x == intersectX)
    {
      return ON_SIDE;
    }
    return point.x > intersectX;
  }
  return 0;
}
bool andriuschin::IsInside::operator()(const Polygon& poly, const Point& point)
{
  using namespace std::placeholders;
  auto adder = std::bind(std::plus<>{}, _1,
        std::bind(details::IsRightOnTheSameHeight{poly.points.back(), point}, _2));
  size_t count = std::accumulate(poly.points.begin(), poly.points.end(), 0, adder);
  return (count == 1) || (count >= details::IsRightOnTheSameHeight::ON_SIDE);
}
bool andriuschin::GetIntersections::operator()(const Polygon& lhs, const Polygon& rhs)
{
  using namespace std::placeholders;
  return std::any_of(lhs.points.cbegin(), lhs.points.cend(), std::bind(IsInside{}, rhs, _1))
      || std::any_of(rhs.points.cbegin(), rhs.points.cend(), std::bind(IsInside{}, lhs, _1));
}
