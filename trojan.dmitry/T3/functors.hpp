#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include "polygon.hpp"
#include "functions.hpp"

struct AreaAccumulator
{
    const Polygon& polygon;
    double operator()(double accum, const Point& point) const;
};
struct EvenAreaSelect
{
    double operator()(const Polygon& polygon) const;
};
struct OddAreaSelect
{
    double operator()(const Polygon& polygon) const;
};
struct MeanAccumulator
{
    double operator()(double accum, const Polygon& polygon) const;
};
struct NumOfVertexesAccumulator
{
    size_t num;
    NumOfVertexesAccumulator(size_t n) :
        num(n)
    {
    }
    double operator()(double accum, const Polygon& polygon) const;
};
struct EvenCountSelecter
{
    size_t operator()(const Polygon& polygon) const;
};
struct OddCountSelecter
{
    size_t operator()(const Polygon& polygon) const;
};
struct VertexCountAccumulator
{
    size_t num;
    VertexCountAccumulator(size_t n) :
        num(n)
    {
    }
    size_t operator()(size_t accum, const Polygon& polygon) const;
};
struct IsRectangleChecker
{
    bool operator()(const Polygon& polygon) const;
};
struct OtherEdgeChecker
{
    const Polygon& target;
    const Polygon& other;
    size_t index;
    OtherEdgeChecker(const Polygon& tar, const Polygon& oth, size_t idx) :
        target(tar),
        other(oth),
        index(idx)
    {
    }
    bool operator()(size_t j) const;
};
struct TargetEdgeChecker
{
    const Polygon& target;
    const Polygon& other;
    TargetEdgeChecker(const Polygon& tar, const Polygon& oth) :
        target(tar),
        other(oth)
    {
    }
    bool operator()(size_t i) const;
};
struct PolygonIntersectionChecker
{
    const Polygon& target;
    PolygonIntersectionChecker(const Polygon& polygon) :
        target(polygon)
    {
    }
    bool operator()(const Polygon& other) const;
};

#endif
