#ifndef CODE_HPP
#define CODE_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <functional>
#include <iomanip>
#include <map>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iterator>
#include <limits>

namespace timofeev
{
  struct Point
  {
    int x;
    int y;
    bool operator==(const Point& other) const {
      return x == other.x && y == other.y;
    }
  };

  struct Polygon
  {
    std::vector< Point > Points;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct IntIO
  {
    int& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, IntIO&& dest);
  std::istream& operator>>(std::istream& in, Polygon& dest);
  double getArea(const Polygon& shape);
  bool isRectangle(const Polygon& shape);
  void AREA(const std::vector< Polygon >& collection, std::string& type);
  void MAX(const std::vector< Polygon >& collection, std::string& type);
  void MIN(const std::vector< Polygon >& collection, std::string& type);
  void COUNT(const std::vector< Polygon >& collection, std::string& type);
  void MAXSEQ(const std::vector< Polygon >& collection, std::string& type);
  void RECTS(const std::vector< Polygon >& collection, std::string& type);
}
#endif
