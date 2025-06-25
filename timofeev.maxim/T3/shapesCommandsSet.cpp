#include "shapesCommandsSet.hpp"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <limits>

namespace timofeev
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, IntIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return in >> dest.ref;
  }

  std::istream& operator>>(std::istream& in, Polygon& Shape)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    Polygon input;
    {
      using sep = DelimiterIO;
      using intg = IntIO;
      size_t count;
      if (!(in >> count))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      if (count < 3)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      for (size_t i = 0; i < count; i++)
      {
        if (in.peek() == '\n') {
          in.setstate(std::ios::failbit);
          return in;
        }
        in >> sep{ '(' };
        Point dot;
        in >> intg{ dot.x } >> sep{ ';' } >> intg{ dot.y };
        input.Points.push_back(dot);
        in >> sep{ ')' };
      }
      if (in.peek() != EOF && in.peek() != '\n')
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    if (in)
    {
      Shape = input;
    }
    return in;
  }
  std::ostream& operator<<(std::ostream& out, const Polygon& Shape)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << Shape.Points.size() << " ";
    for (size_t i = 0; i < Shape.Points.size(); i++)
    {
      out << "(";
      out << Shape.Points[i].x << "; " << Shape.Points[i].y;
      out << ") ";
    }
    out << "area: " << timofeev::getArea(Shape);
    out << " rectangle?: " << std::boolalpha << timofeev::isRectangle(Shape);
    return out;
  }

  double getArea(const timofeev::Polygon& shape)
  {
    double area = 0;
    for (size_t i = 0; i < shape.Points.size(); i++)
    {
      area += shape.Points[i].x * shape.Points[(i + 1) % shape.Points.size()].y;
      area -= shape.Points[(i + 1) % shape.Points.size()].x * shape.Points[i].y;
    }
    return std::abs(area) * 0.5;
  }
  bool isRectangle(const Polygon& shape)
  {
    if (shape.Points.size() != 4)
    {
      return false;
    }
    auto vec = [](Point a, Point b) {
      return pow(pow((b.x - a.x), 2) + pow((b.y - a.y), 2), 0.5);
      };

    auto rightTriangle = [](double a, double b, double c) {
      double linesSum = pow(a, 2) + pow(b, 2) + pow(c, 2);
      double gip = pow(std::max(std::max(a, b), c), 2);
      if (gip == linesSum - gip)
      {
        return false;
      }
      return true;
      };

    for (size_t i = 0; i < 3; i++)
    {
      double line1 = vec(shape.Points[i % shape.Points.size()], shape.Points[(i + 1) % shape.Points.size()]);
      double line2 = vec(shape.Points[(i + 1) % shape.Points.size()], shape.Points[(i + 2) % shape.Points.size()]);
      double line3 = vec(shape.Points[(i + 2) % shape.Points.size()], shape.Points[i % shape.Points.size()]);
      if (!rightTriangle(line1, line2, line3))
      {
        return false;
      }
    }
    return true;
  }

  size_t MaxSequenceCounter::operator()(const std::vector<Polygon>& collection) const
  {
    size_t maxSeq = 0;
    size_t currentSeq = 0;

    for (const auto& poly : collection)
    {
      if (isMatch(poly))
      {
        currentSeq++;
        maxSeq = std::max(maxSeq, currentSeq);
      }
      else
      {
        currentSeq = 0;
      }
    }

    return maxSeq;
  }

  bool MaxSequenceCounter::isMatch(const Polygon& other) const
  {
    return other.Points.size() == shape.Points.size() &&
      std::is_permutation(other.Points.begin(), other.Points.end(), shape.Points.begin());
  }

  void area(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out)
  {
    std::string type;
    in >> type;
    if (type == "EVEN")
    {
      double sumOfArea = std::accumulate(collection.begin(), collection.end(), 0.0,
        [](double acc, Polygon shape) {return acc + (shape.Points.size() % 2 == 0 ? getArea(shape) : 0); });
      out << std::fixed << std::setprecision(1) << sumOfArea << '\n';
    }
    else if (type == "ODD")
    {
      double sumOfArea = std::accumulate(collection.begin(), collection.end(), 0.0,
        [](double acc, Polygon shape) {return acc + (shape.Points.size() % 2 == 1 ? getArea(shape) : 0); });
      out << std::fixed << std::setprecision(1) << sumOfArea << '\n';
    }
    else if (type == "MEAN")
    {
      if (collection.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
      }

      double sumOfArea = std::accumulate(collection.begin(), collection.end(), 0.0,
        [](double acc, Polygon shape) {return acc + getArea(shape); });
      out << std::fixed << std::setprecision(1) << sumOfArea / collection.size() << '\n';
    }
    else
    {
      size_t num = std::stoul(type);
      if (num < 3)
      {
        throw std::invalid_argument("");
      }
      double sumOfArea = std::accumulate(collection.begin(), collection.end(), 0.0,
        [&num](double acc, Polygon shape) {return acc + (shape.Points.size() == num ? getArea(shape) : 0); });
      out << std::fixed << std::setprecision(1) << sumOfArea << '\n';
    }
  }

  void max(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out)
  {
    std::string type;
    in >> type;
    if (collection.size() == 0)
    {
      throw std::invalid_argument("");
    }

    if (type == "AREA")
    {
      auto maxArea = std::max_element(collection.begin(), collection.end(),
        [](const Polygon& a, const Polygon& b) {return getArea(a) < getArea(b); });
      out << std::fixed << std::setprecision(1) << getArea(*maxArea) << '\n';
    }
    else if (type == "VERTEXES")
    {
      auto maxVertexes = std::max_element(collection.begin(), collection.end(),
        [](const Polygon& a, const Polygon& b) {return a.Points.size() < b.Points.size(); });
      out << std::fixed << std::setprecision(0) << maxVertexes->Points.size() << '\n';
    }
    else
    {
      throw std::invalid_argument("");
    }
  }

  void min(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out)
  {
    std::string type;
    in >> type;
    if (collection.size() == 0)
    {
      throw std::invalid_argument("");
    }

    if (type == "AREA")
    {
      auto maxArea = std::min_element(collection.begin(), collection.end(),
        [](const Polygon& a, const Polygon& b) {return getArea(a) < getArea(b); });
      out << std::fixed << std::setprecision(1) << getArea(*maxArea) << '\n';
    }
    else if (type == "VERTEXES")
    {
      auto maxArea = std::min_element(collection.begin(), collection.end(),
        [](const Polygon& a, const Polygon& b) {return a.Points.size() < b.Points.size(); });
      out << std::fixed << std::setprecision(0) << getArea(*maxArea) << '\n';
    }
    else
    {
      throw std::invalid_argument("");
    }
  }

  void count(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out)
  {
    std::string type;
    in >> type;
    if (type == "EVEN")
    {
      size_t vCount = std::count_if(collection.begin(), collection.end(),
        [](const Polygon& shape) {return shape.Points.size() % 2 == 0; });
      out << vCount << '\n';
    }
    else if (type == "ODD")
    {
      size_t vCount = std::count_if(collection.begin(), collection.end(),
        [](const Polygon& shape) {return shape.Points.size() % 2 == 0; });
      out << vCount << '\n';
    }

    else
    {
      size_t num = std::stoul(type);
      if (num < 3)
      {
        throw std::invalid_argument("");
      }
      size_t countV = std::count_if(collection.begin(), collection.end(),
        [&num](const Polygon& shape) {return shape.Points.size() % 2 == num; });
      out << countV << '\n';
    }
  }

  void maxseq(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out)
  {
    Polygon shape;
    if (!(in >> shape))
    {
      throw std::invalid_argument("");
    }
    MaxSequenceCounter counter(shape);
    out << counter(collection) << '\n';
  }

  void rects(const std::vector< Polygon >& collection, std::istream& in, std::ostream& out)
  {
    (void)in;
    size_t count = std::count_if(collection.begin(), collection.end(), isRectangle);
    out << count << '\n';
  }
}
