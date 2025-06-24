#include "commands.hpp"
#include "functors.hpp"
#include "functions.hpp"
#include <numeric>

double getArea(const Polygon& polygon)
{
    if (polygon.points.size() < 3) {
        return 0.0;
    }
    double area = std::accumulate(polygon.points.begin(), polygon.points.end(), 0.0, AreaAccumulator{polygon});
    return std::abs(area) / 2.0;
}
int orientation(const Point& p, const Point& q, const Point& r)
{
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) {
        return 0;
    }
    return (val > 0) ? 1 : 2;
}
bool onSegment(const Point& p, const Point& q, const Point& r)
{
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
}
bool segmentsIntersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2)
{
    int o1 = orientation(p1, p2, q1);
    int o2 = orientation(p1, p2, q2);
    int o3 = orientation(q1, q2, p1);
    int o4 = orientation(q1, q2, p2);

    if (o1 != o2 && o3 != o4) {
        return true;
    }
    if (o1 == 0 && onSegment(p1, q1, p2)) {
        return true;
    }
    if (o2 == 0 && onSegment(p1, q2, p2)) {
        return true;
    }
    if (o3 == 0 && onSegment(q1, p1, q2)) {
        return true;
    }
    if (o4 == 0 && onSegment(q1, p2, q2)) {
        return true;
    }

    return false;
}