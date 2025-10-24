#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"
#include <vector>

bool area(const std::vector< Polygon >& polygons);
bool maxx(const std::vector< Polygon >& polygons);
bool minn(const std::vector< Polygon >& polygons);
bool count(const std::vector< Polygon >& polygons);
bool rects(const std::vector< Polygon >& polygons);
bool intersections(const std::vector< Polygon >& polygons);

#endif
