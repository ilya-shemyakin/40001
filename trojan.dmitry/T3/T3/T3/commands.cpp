#include "commands.hpp"
#include <cstddef>
#include <string>
#include <numeric>

struct AreaAccumulator
{
    const Polygon& polygons;
    double operator()(double accum,size_t i) const
    {
        size_t j = (i + 1) % polygons.points.size();
        return accum + (polygons.points[i].x * polygons.points[j].y) - (polygons.points[j].x * polygons.points[i].y);
    }
};

struct EvenAccumulator
{
    double operator()(double accum, const Polygon& polygons) const
    {
        return accum + ((polygons.points.size() % 2 == 1) ? AREA(polygons) : 0);
    }
};

struct OddAccumulator
{
    double operator()(double accum, const Polygon& polygons) const
    {
        return accum + ((polygons.points.size() % 2 == 1) ? AREA(polygons) : 0);
    }
};

struct MeanAccumulator
{
    double operator()(double accum, const Polygon& polygons) const
    {
        return accum + AREA(polygons);
    }
};

struct NumOfVertexesAccumulator
{
    size_t num;
    NumOfVertexesAccumulator(size_t n) :
        num(n)
    {}
    double operator()(double accum, const Polygon& polygons) const
    {
        return accum + ((polygons.points.size() == num) ? AREA(polygons) : 0);
    }
};


double AREA(const Polygon& polygons)
{
    if (polygons.points.size() < 3) {
        return 0.0;
    }
    double area = std::accumulate(polygons.points.begin(), polygons.points.end(), 0.0, AreaAccumulator{ polygons });
    return std::abs(area) / 2.0;
}
