#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <vector>
#include <sstream>
#include "Polygon.h"

std::istream& operator>>(std::istream& in, Point& p);
std::istream& operator>>(std::istream& in, Polygon& poly);
void invalComm();
bool isEmpty(polys& polys);

double calculateArea(std::vector<Point>& points);
double areaEvenOdd(const std::string& arg, const polys& polys);
double areaMean(const polys& polys);
double areaNum(size_t arg, const polys& polys);
bool VertexCountComparator(const Polygon& a, const Polygon& b);
bool AreaComparator(const Polygon& a, const Polygon& b);
double maxAreaVer(const std::string& arg, const polys& polys);
double minAreaVer(const std::string& arg, const polys& polys);
int countVer(const std::string& arg, const polys& polys);
int countVer(size_t arg, const polys& polys);
int echo(polys& data, const Polygon& target);
bool hasRightAngle(const std::vector<Point>& figure);
int rightShapes(const polys& polys);

#endif
