#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "polygon.hpp"

double getArea(const Polygon& polygon);
int orientation(const Point& p, const Point& q, const Point& r);
bool onSegment(const Point& p, const Point& q, const Point& r);
bool segmentsIntersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2);

#endif
