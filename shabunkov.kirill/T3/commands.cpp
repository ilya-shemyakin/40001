#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>
#include <functional>
#include <iomanip>
#include <limits>
#include <cstddef>

#include "commands.h"
#include "Polygon.h"

void invalidCommand()
{
  std::cout << "<INVALID COMMAND>\n";
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isEmpty(const std::vector<Polygon>& polys)
{
  return polys.empty();
}

std::istream& operator>>(std::istream& in, Point& p)
{
  char symbol;
  if (in >> symbol && symbol == '('
    && in >> p.x >> symbol
    && symbol == ';'
    && in >> p.y >> symbol
    && symbol == ')')
  {
    return in;
  }
  in.setstate(std::ios::failbit);
  return in;
}

std::istream& operator>>(std::istream& in, Polygon& poly) {
  poly.points.clear();
  int count;

  if (!(in >> count) || (count < 3))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::generate_n(std::back_inserter(poly.points), count, [&in]()
  {
    Point p;
    in >> p;
    return p;
  }
  );

  if (in.fail() || (in.peek() != EOF && in.peek() != '\n'))
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

double calculateArea(const std::vector<Point>& points)
{
  auto op = [&points](double sum, size_t i) {
    size_t j = (i + 1) % points.size();
    return sum + (points[i].x * points[j].y) - (points[j].x * points[i].y);
    };

  std::vector<size_t> indices(points.size());
  std::iota(indices.begin(), indices.end(), 0);

  double sum = std::accumulate(
    indices.begin(),
    indices.end(),
    0.0,
    op
  );

  return std::abs(sum) / 2.0;
}

double areaEvenOdd(const std::string& parametr, const std::vector<Polygon>& polygons)
{
  bool checkEven = (parametr == "EVEN");

  double areaSum = std::accumulate(
    polygons.begin(),
    polygons.end(),
    0.0,
    [checkEven](double sum, const Polygon& poly)
    {
      bool isEven = (poly.points.size() % 2 == 0);
      if (isEven == checkEven)
      {
        return sum + calculateArea(poly.points);
      }
      return sum;
    }
  );

  return areaSum;
}

double areaMean(const std::vector<Polygon>& polys)
{
  double result = std::accumulate(
    polys.begin(),
    polys.end(),
    0.0,
    [](double sum, const Polygon& poly)
    {
      return sum + calculateArea(poly.points);
    }
  );

  return result / static_cast<double>(polys.size());
}

double areaNumOfVertex(size_t vertex, const std::vector<Polygon>& polys)
{
  double result = std::accumulate(
    polys.begin(),
    polys.end(),
    0.0,
    [vertex](double sum, const Polygon& poly)
    {
      if (poly.points.size() == vertex)
      {
        return sum + calculateArea(poly.points);
      }
      return sum;
    }
  );

  return result;
}

bool countVertexComparator(const Polygon& first, const Polygon& second)
{
  return first.points.size() < second.points.size();
}

bool areaComparator(const Polygon& first, const Polygon& second)
{
  return calculateArea(first.points) < calculateArea(second.points);
}

double maxAreaVertexes(const std::string& parametr, const std::vector<Polygon>& polys)
{
  if (parametr == "AREA")
  {
    auto max = std::max_element(
      polys.begin(),
      polys.end(),
      [](const Polygon& first, const Polygon& second)
      {
        return calculateArea(first.points) < calculateArea(second.points);
      }
    );

    return calculateArea(max->points);
  }
  else if (parametr == "VERTEXES")
  {
    auto max = std::max_element(
      polys.begin(),
      polys.end(),
      [](const Polygon& first, const Polygon& second)
      {
        return first.points.size() < second.points.size();
      }
    );

    return static_cast<double>(max->points.size());
  }

  invalidCommand();
  return 0.0;
}

double minAreaVertexes(const std::string& parametr, const std::vector<Polygon>& polys)
{
  if (parametr == "AREA")
  {
    auto min = std::min_element(
      polys.begin(),
      polys.end(),
      [](const Polygon& first, const Polygon& second)
      {
        return calculateArea(first.points) < calculateArea(second.points);
      }
    );

    return calculateArea(min->points);
  }
  else if (parametr == "VERTEXES")
  {
    auto min = std::min_element(
      polys.begin(),
      polys.end(),
      [](const Polygon& first, const Polygon& second)
      {
        return first.points.size() < second.points.size();
      }
    );

    return static_cast<double>(min->points.size());
  }

  invalidCommand();
  return 0.0;
}

size_t countEvenOdd(const std::string& parametr, const std::vector<Polygon>& polys)
{
  bool checkEven = (parametr == "EVEN");

  size_t result = std::count_if(
    polys.begin(),
    polys.end(),
    [checkEven](const Polygon& poly)
    {
      bool isEven = (poly.points.size() % 2 == 0);
      return isEven == checkEven;
    }
  );

  return result;
}

int countNumOfVertexes(int vertexes, const std::vector<Polygon>& polys)
{
  if (vertexes < 3)
  {
    invalidCommand();
    return -1;
  }

  int result = std::count_if(
    polys.begin(),
    polys.end(),
    [vertexes](const Polygon& poly)
    {
      return poly.points.size() == static_cast<size_t>(vertexes);
    }
  );

  return result;
}

auto getBoundingBox(const std::vector<Polygon>& polys,
  int& minX, int& maxX, int& minY, int& maxY)
{
  minX = std::numeric_limits<int>::max();
  minY = std::numeric_limits<int>::max();
  maxX = std::numeric_limits<int>::min();
  maxY = std::numeric_limits<int>::min();

  auto updateBounds = [&](const Point& p) {
    minX = std::min(minX, p.x);
    minY = std::min(minY, p.y);
    maxX = std::max(maxX, p.x);
    maxY = std::max(maxY, p.y);
    };

  std::accumulate(polys.begin(), polys.end(), 0,
    [&](int, const Polygon& poly) {
      std::accumulate(poly.points.begin(), poly.points.end(), 0,
        [&](int, const Point& p) {
          updateBounds(p);
          return 0;
        });
      return 0;
    });
}

bool inFrame(const std::vector<Polygon>& polys, const Polygon& target)
{
  using namespace std::placeholders;

  if (target.points.size() < 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return false;
  }

  if (polys.empty())
  {
    return false;
  }

  int min_x, min_y, max_x, max_y;
  getBoundingBox(polys, min_x, min_y, max_x, max_y);

  auto isInFrame = std::bind(
    PointInFrameChecker(min_x, min_y, max_x, max_y),
    _1
  );

  bool allInFrame = std::all_of(target.points.begin(), target.points.end(), isInFrame);
  std::cout << (allInFrame ? "<TRUE>" : "<FALSE>") << "\n";
  return allInFrame;
}

bool hasRightAngle(const std::vector<Point>& point)
{
  if (point.size() < 3) return false;

  return std::any_of(point.begin(), point.end(), AngleChecker{ point });
}

size_t rightShapes(const std::vector<Polygon>& polys) {
  return std::count_if(polys.begin(), polys.end(),
    [](const Polygon& poly) {
      return hasRightAngle(poly.points);
    });
}

void processCommands(const std::vector<Polygon>& polygons)
{
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      if (command == "AREA")
      {
        std::string param;
        if (!(std::cin >> param))
        {
          invalidCommand();
          continue;
        }

        if (param == "EVEN" || param == "ODD")
        {
          double area = areaEvenOdd(param, polygons);
          std::cout << std::fixed << std::setprecision(1) << area << '\n';
        }
        else if (param == "MEAN")
        {
          if (polygons.empty())
          {
            invalidCommand();
            continue;
          }
          double area = areaMean(polygons);
          std::cout << std::fixed << std::setprecision(1) << area << '\n';
        }
        else
        {
          try
          {
            size_t num = std::stoul(param);
            if (num < 3)
            {
              invalidCommand();
              continue;
            }
            double area = areaNumOfVertex(num, polygons);
            std::cout << std::fixed << std::setprecision(1) << area << '\n';
          }
          catch (...)
          {
            invalidCommand();
          }
        }
      }
      else if (command == "MAX")
      {
        std::string param;
        if (!(std::cin >> param))
        {
          invalidCommand();
          continue;
        }

        if (polygons.empty())
        {
          invalidCommand();
          continue;
        }

        if (param == "AREA")
        {
          double result = maxAreaVertexes(param, polygons);
          std::cout << std::fixed << std::setprecision(1) << result << '\n';
        }
        else if (param == "VERTEXES")
        {
          auto max = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& first, const Polygon& second)
            {
              return first.points.size() < second.points.size();
            });
          std::cout << max->points.size() << '\n';
        }
        else
        {
          invalidCommand();
        }
      }
      else if (command == "MIN")
      {
        std::string param;
        if (!(std::cin >> param))
        {
          invalidCommand();
          continue;
        }

        if (polygons.empty())
        {
          invalidCommand();
          continue;
        }

        if (param == "AREA")
        {
          double result = minAreaVertexes(param, polygons);
          std::cout << std::fixed << std::setprecision(1) << result << '\n';
        }
        else if (param == "VERTEXES")
        {
          auto min = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& first, const Polygon& second)
            {
              return first.points.size() < second.points.size();
            });
          std::cout << min->points.size() << '\n';
        }
        else
        {
          invalidCommand();
        }
      }
      else if (command == "COUNT")
      {
        std::string param;
        if (!(std::cin >> param))
        {
          invalidCommand();
          continue;
        }

        if (param == "EVEN" || param == "ODD")
        {
          size_t count = countEvenOdd(param, polygons);
          std::cout << count << '\n';
        }
        else
        {
          try
          {
            int num = std::stoi(param);
            if (num < 3)
            {
              invalidCommand();
              continue;
            }
            size_t count = countNumOfVertexes(num, polygons);
            std::cout << count << '\n';
          }
          catch (...)
          {
            invalidCommand();
          }
        }
      }
      else if (command == "INFRAME")
      {
        Polygon target;
        if (!(std::cin >> target))
        {
          invalidCommand();
          continue;
        }
        bool result = inFrame(polygons, target);
        std::cout << (result ? "<TRUE>" : "<FALSE>") << '\n';
      }
      else if (command == "RIGHTSHAPES")
      {
        size_t count = rightShapes(polygons);
        std::cout << count << '\n';
      }
      else if (command == "ECHO" || command == "RMECHO" ||
        command == "MAXSEQ" || command == "PERMS" ||
        command == "INTERSECTIONS" || command == "SAME" ||
        command == "RECTS" || command == "LESSAREA")
      {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        invalidCommand();
      }
      else
      {
        invalidCommand();
      }
    }
    catch (...)
    {
      invalidCommand();
    }
  }
}

