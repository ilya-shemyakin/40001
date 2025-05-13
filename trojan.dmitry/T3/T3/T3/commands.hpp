#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"
#include <vector>

void AREA(const std::vector< Polygon >& polygons, const std::string& parameter);
void MAX(const std::vector< Polygon >& polygons, const std::string& parameter);
void MIN(const std::vector< Polygon >& polygons, const std::string& parameter);
void COUNT(const std::vector< Polygon >& polygons, const std::string& parameter);
void RECTS(const std::vector< Polygon >& polygons);
void INTERSECTIONS(const std::vector< Polygon >& polygons, const Polygon& target);

#endif
