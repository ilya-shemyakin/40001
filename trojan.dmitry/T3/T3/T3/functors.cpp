#include "functions.hpp"
#include "functors.hpp"
#include <algorithm>
#include <numeric>

double AreaAccumulator::operator()(double accum, const Point& point) const
{
    size_t i = &point - polygon.points.data();
    size_t j = (i + 1) % polygon.points.size();
    return accum + (point.x * polygon.points[j].y) - (polygon.points[j].x * point.y);
}

double EvenAccumulator::operator()(double accum, const Polygon& polygon) const
{
    return accum + ((polygon.points.size() % 2 == 1) ? getArea(polygon) : 0);
}

double OddAccumulator::operator()(double accum, const Polygon& polygon) const
{
    return accum + ((polygon.points.size() % 2 == 1) ? getArea(polygon) : 0);
}
double MeanAccumulator::operator()(double accum, const Polygon& polygon) const
{
    return accum + getArea(polygon);
}
double NumOfVertexesAccumulator::operator()(double accum, const Polygon& polygon) const
{
    return accum + ((polygon.points.size() == num) ? getArea(polygon) : 0);
}
bool MaxAreaComparator::operator()(const Polygon& lhs, const Polygon& rhs) const
{
    return getArea(lhs) < getArea(rhs);
}
bool MaxOfVertexesComparator::operator()(const Polygon& lhs, const Polygon& rhs) const
{
    return lhs.points.size() < rhs.points.size();
}
bool MinAreaComparator::operator()(const Polygon& lhs, const Polygon& rhs) const
{
    return getArea(lhs) < getArea(rhs);
}
bool MinOfVertexesComparator::operator()(const Polygon& lhs, const Polygon& rhs) const
{
    return lhs.points.size() < rhs.points.size();
}
size_t EvenCountAccumulator::operator()(size_t accum, const Polygon& polygon) const
{
    return accum + ((polygon.points.size() % 2 == 0) ? 1 : 0);
}
size_t OddCountAccumulator::operator()(size_t accum, const Polygon& polygon) const
{
    return accum + ((polygon.points.size() % 2 == 1) ? 1 : 0);
}
size_t VertexCountAccumulator::operator()(size_t accum, const Polygon& polygon) const
{
    return accum + ((polygon.points.size() == num) ? 1 : 0);
}
bool IsRectangleChecker::operator()(const Polygon& polygon) const
{
    if (polygon.points.size() != 4) {
        return false;
    }
    auto& p = polygon.points;
    int dx1 = p[1].x - p[0].x;
    int dy1 = p[1].y - p[0].y;
    int dx2 = p[2].x - p[1].x;
    int dy2 = p[2].y - p[1].y;
    int dx3 = p[3].x - p[2].x;
    int dy3 = p[3].y - p[2].y;
    int dx4 = p[0].x - p[3].x;
    int dy4 = p[0].y - p[3].y;

    return (dx1 * dx2 + dy1 * dy2 == 0) &&
        (dx2 * dx3 + dy2 * dy3 == 0) &&
        (dx3 * dx4 + dy3 * dy4 == 0) &&
        (dx4 * dx1 + dy4 * dy1 == 0);
}
bool OtherEdgeChecker::operator()(size_t j) const
{
    const Point& a1 = target.points[index];
    const Point& a2 = target.points[(index + 1) % target.points.size()];
    const Point& b1 = other.points[j];
    const Point& b2 = other.points[(j + 1) % other.points.size()];
    return segmentsIntersect(a1, a2, b1, b2);
}
bool TargetEdgeChecker::operator()(size_t i) const
{
    std::vector<size_t> indices(other.points.size());
    std::iota(indices.begin(), indices.end(), 0);
    return std::any_of(indices.begin(), indices.end(), OtherEdgeChecker(target, other, i));
}
bool PolygonIntersectionChecker::operator()(const Polygon& other) const
{
    if (target.points.empty() || other.points.empty()) {
        return false;
    }
    std::vector<size_t> indices(target.points.size());
    std::iota(indices.begin(), indices.end(), 0);
    return std::any_of(indices.begin(), indices.end(), TargetEdgeChecker(target, other));
}
