#ifndef COMMANDS_H
#define COMMANDS_H

#include <sstream>
#include <vector>
#include <string>

#include "Polygon.h"

std::istream& operator>>(std::istream& in, Point& point);
std::istream& operator>>(std::istream& in, Polygon& polygon);

void invalidCommand();
bool isEmpty(const std::vector<Polygon>& polys);
double calculateArea(const std::vector<Point>& points);
double areaEvenOdd(const std::string& parametr, const std::vector<Polygon>& polygon);
double areaMean(const std::vector<Polygon>& polys);
double areaNumOfVertex(size_t vertex, const std::vector<Polygon>& polys);
double maxAreaVertexes(const std::string& parametr, const std::vector<Polygon>& polys);
double minAreaVertexes(const std::string& parametr, const std::vector<Polygon>& polys);
size_t countEvenOdd(const std::string& parametr, const std::vector<Polygon>& polys);
int countNumOfVertexes(int vertexes, const std::vector<Polygon>& polys);
auto getBoundingBox(const std::vector<Polygon>& polys, int& minX,
  int& maxX, int& minY, int& maxY);
bool inFrame(const std::vector<Polygon>& polys, const Polygon& target);
size_t rightShapes(const std::vector<Polygon>& polys);
bool hasRightAngle(const std::vector<Point>& point);
void processCommands(const std::vector<Polygon>& polygons);
#endif

