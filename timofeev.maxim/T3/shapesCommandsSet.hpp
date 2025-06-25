#ifndef SHAPESCOMMANDSSET_HPP
#define SHAPESCOMMANDSSET_HPP

#include <iostream>
#include <vector>
#include <string>

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

  struct MaxSequenceCounter
  {
    Polygon shape;

    MaxSequenceCounter(const Polygon& s) : shape(s) {}

    size_t operator()(const std::vector<Polygon>& collection) const;

  private:
    bool isMatch(const Polygon& other) const;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, IntIO&& dest);
  std::istream& operator>>(std::istream& in, Polygon& dest);
  double getArea(const Polygon& shape);
  bool isRectangle(const Polygon& shape);
  void area(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out);
  void max(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out);
  void min(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out);
  void count(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out);
  void maxseq(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out);
  void rects(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out);
}
#endif
