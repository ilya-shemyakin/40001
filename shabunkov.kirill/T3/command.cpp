#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>
#include <functional>
#include <iomanip>

#include "commands.h"
#include "Polygon.h"

void invalidCommand()
{
  std::cout << "<INVALID COMMAND>\n";
  std::cin.clear();
  std::cin.ignore();
}

bool isEmpty(const std::vector<Polygon>& polys)
{
  return polys.size() == 0;
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

  for (int i = 0; i < count; ++i)
  {
    if (in.peek() == '\n') {
      in.setstate(std::ios::failbit);
      return in;
    }
    Point p;
    if (!(in >> p)) {
      return in;
    }
    poly.points.push_back(p);
  }

  if (in.peek() != EOF && in.peek() != '\n')
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

  return 0.0;
}

int countEvenOdd(const std::string& parametr, const std::vector<Polygon>& polys)
{
  bool checkEven = (parametr == "EVEN");

  int result = std::count_if(
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
      return poly.points.size() == vertexes;
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

  if (polys.empty() || target.points.size() == 0)
  {
    invalidCommand();
    return false;
  }

  int min_x, min_y, max_x, max_y;
  getBoundingBox(polys, min_x, min_y, max_x, max_y);

  auto isInFrame = std::bind(
    PointInFrameChecker(min_x, min_y, max_x, max_y),
    std::placeholders::_1
  );

  return std::all_of(target.points.begin(), target.points.end(), isInFrame);
}

bool hasRightAngle(const std::vector<Point>& point)
{
  if (point.size() < 3) return false;

  return std::any_of(point.begin(), point.end(), AngleChecker{ point });
}

int rightShapes(const std::vector<Polygon>& polys) {
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
        std::cin >> param;
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
        std::cin >> param;
        if (polygons.empty()) {
          invalidCommand();
          continue;
        }
        double result = maxAreaVertexes(param, polygons);
        std::cout << std::fixed << std::setprecision(1) << result << '\n';
      }
      else if (command == "MIN")
      {
        std::string param;
        std::cin >> param;
        if (polygons.empty())
        {
          invalidCommand();
          continue;
        }
        double result = minAreaVertexes(param, polygons);
        std::cout << std::fixed << std::setprecision(1) << result << '\n';
      }
      else if (command == "COUNT")
      {
        std::string param;
        std::cin >> param;
        if (param == "EVEN" || param == "ODD")
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>
#include <functional>
#include <iomanip>

#include "commands.h"
#include "Polygon.h"

void invalidCommand()
{
  std::cout << "<INVALID COMMAND>\n";
  std::cin.clear();
  std::cin.ignore();
}

bool isEmpty(const std::vector<Polygon>& polys)
{
  return polys.size() == 0;
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

  for (int i = 0; i < count; ++i)
  {
    if (in.peek() == '\n') {
      in.setstate(std::ios::failbit);
      return in;
    }
    Point p;
    if (!(in >> p)) {
      return in;
    }
    poly.points.push_back(p);
  }

  if (in.peek() != EOF && in.peek() != '\n')
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

  return 0.0;
}

int countEvenOdd(const std::string& parametr, const std::vector<Polygon>& polys)
{
  bool checkEven = (parametr == "EVEN");

  int result = std::count_if(
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
      return poly.points.size() == vertexes;
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

  if (polys.empty() || target.points.size() == 0)
  {
    invalidCommand();
    return false;
  }

  int min_x, min_y, max_x, max_y;
  getBoundingBox(polys, min_x, min_y, max_x, max_y);

  auto isInFrame = std::bind(
    PointInFrameChecker(min_x, min_y, max_x, max_y),
    std::placeholders::_1
  );

  return std::all_of(target.points.begin(), target.points.end(), isInFrame);
}

bool hasRightAngle(const std::vector<Point>& point)
{
  if (point.size() < 3) return false;

  return std::any_of(point.begin(), point.end(), AngleChecker{ point });
}

int rightShapes(const std::vector<Polygon>& polys) {
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
        std::cin >> param;
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
        std::cin >> param;
        if (polygons.empty()) {
          invalidCommand();
          continue;
        }
        double result = maxAreaVertexes(param, polygons);
        std::cout << std::fixed << std::setprecision(1) << result << '\n';
      }
      else if (command == "MIN")
      {
        std::string param;
        std::cin >> param;
        if (polygons.empty())
        {
          invalidCommand();
          continue;
        }
        double result = minAreaVertexes(param, polygons);
        std::cout << std::fixed << std::setprecision(1) << result << '\n';
      }
      else if (command == "COUNT")
      {
        std::string param;
        std::cin >> param;
        if (param == "EVEN" || param == "ODD")
        {
          int count = countEvenOdd(param, polygons);
          std::cout << count << '\n';
        }
        else
        {
          try
          {
            int num = std::stoi(param);
            int count = countNumOfVertexes(num, polygons);
            if (count != -1)
            {
              std::cout << count << '\n';
            }
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
        std::cin >> target;
        bool result = inFrame(polygons, target);
        std::cout << (result ? "<TRUE>" : "<FALSE>") << '\n';
      }
      else if (command == "RIGHTSHAPES")
      {
        int count = rightShapes(polygons);
        std::cout << count << '\n';
      }
      else {
        invalidCommand();
      }
    }
    catch (...)
    {
      invalidCommand();
    }
  }
}

