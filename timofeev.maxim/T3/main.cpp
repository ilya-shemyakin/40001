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
      for (size_t i = 0; i < count; i++)
      {
        in >> sep{ '(' };
        Point dot;
        in >> intg{ dot.x } >> sep{ ';' } >> intg{ dot.y };
        input.Points.push_back(dot);
        in >> sep{ ')' };
      }
      if (in.peek() != EOF && in.peek() != '\n')
      {
        in.setstate(std::ios::failbit);
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



  void AREA(const std::vector< Polygon >& collection, std::string& type)
  {
    if (type == "EVEN")
    {
      double sumOfArea = std::accumulate(collection.begin(), collection.end(), 0.0,
        [](double acc, Polygon shape) {return acc + (shape.Points.size() % 2 == 0 ? getArea(shape) : 0); });
      std::cout << std::fixed << std::setprecision(1) << sumOfArea << '\n';
    }
    else if (type == "ODD")
    {
      double sumOfArea = std::accumulate(collection.begin(), collection.end(), 0.0,
        [](double acc, Polygon shape) {return acc + (shape.Points.size() % 2 == 1 ? getArea(shape) : 0); });
      std::cout << std::fixed << std::setprecision(1) << sumOfArea << '\n';
    }
    else if (type == "MEAN")
    {
      if (collection.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
      }

      double sumOfArea = std::accumulate(collection.begin(), collection.end(), 0.0,
        [](double acc, Polygon shape) {return acc + getArea(shape); });
      std::cout << std::fixed << std::setprecision(1) << sumOfArea / collection.size() << '\n';
    }
    else
    {
      try
      {
        size_t num = std::stoul(type);
        if(num < 3)
        {
          throw std::invalid_argument("");
        }
        double sumOfArea = std::accumulate(collection.begin(), collection.end(), 0.0,
          [&num](double acc, Polygon shape) {return acc + (shape.Points.size() == num ? getArea(shape) : 0);});
        std::cout << std::fixed << std::setprecision(1) << sumOfArea << '\n';
      }
      catch (const std::invalid_argument& e)
      {
        throw;
      }
    }
  }

  void MAX(const std::vector< Polygon >& collection, std::string& type)
  {
    try
    {
      if (collection.size() == 0)
      {
        throw std::invalid_argument("");
      }

      if (type == "AREA")
      {
        auto maxArea = std::max_element(collection.begin(), collection.end(),
          [](const Polygon& a, const Polygon& b) {return getArea(a) < getArea(b); });
        std::cout << std::fixed << std::setprecision(1) << getArea(*maxArea) << '\n';
      }
      else if (type == "VERTEXES")
      {
        auto maxVertexes = std::max_element(collection.begin(), collection.end(),
          [](const Polygon& a, const Polygon& b) {return a.Points.size() < b.Points.size(); });
        std::cout << std::fixed << std::setprecision(0) << maxVertexes->Points.size() << '\n';
      }
      else
      {
        throw std::invalid_argument("");
      }
    }
    catch (const std::invalid_argument& e)
    {
      throw;
    }
  }

  void MIN(const std::vector< Polygon >& collection, std::string& type)
  {
    try
    {
      if (collection.size() == 0)
      {
        throw std::invalid_argument("");
      }

      if (type == "AREA")
      {
        auto maxArea = std::min_element(collection.begin(), collection.end(),
          [](const Polygon& a, const Polygon& b) {return getArea(a) < getArea(b); });
        std::cout << std::fixed << std::setprecision(1) << getArea(*maxArea) << '\n';
      }
      else if (type == "VERTEXES")
      {
        auto maxArea = std::min_element(collection.begin(), collection.end(),
          [](const Polygon& a, const Polygon& b) {return a.Points.size() < b.Points.size(); });
        std::cout << std::fixed << std::setprecision(0) << getArea(*maxArea) << '\n';
      }
      else
      {
        throw std::invalid_argument("");
      }
    }
    catch (const std::invalid_argument& e)
    {
      throw;
    }
  }

  void COUNT(const std::vector< Polygon >& collection, std::string& type)
  {
    if (type == "EVEN")
    {
      size_t vCount = std::accumulate(collection.begin(), collection.end(), 0,
        [](double acc, Polygon shape) {return acc + (shape.Points.size() % 2 == 0 ? 1 : 0); });
      std::cout << vCount << '\n';
    }
    else if (type == "ODD")
    {
      size_t vCount = std::accumulate(collection.begin(), collection.end(), 0,
        [](double acc, Polygon shape) {return acc + (shape.Points.size() % 2 == 1 ? 1 : 0); });
      std::cout << vCount << '\n';
    }

    else
    {
      try
      {
        size_t num = std::stoul(type);
        if(num < 3)
        {
          throw std::invalid_argument("");
        }
        size_t countV = std::accumulate(collection.begin(), collection.end(), 0,
          [&num](double acc, Polygon shape) {return acc + (shape.Points.size() == num ? 1 : 0); });
        std::cout << countV << '\n';
      }
      catch (const std::invalid_argument& e)
      {
        throw;
      }
    }
  }

  void MAXSEQ(const std::vector< Polygon >& collection, std::string& type)
  {
    try
    {
      std::istringstream iss(type);
      Polygon shape;
      if (!(iss >> shape))
      {
        throw std::invalid_argument("");
      }
      size_t maxSeq = 0;
      size_t currentSeq = 0;
      for (size_t i = 0; i < collection.size(); i++)
      {
        if (collection[i].Points.size() == shape.Points.size() &&
          std::is_permutation(collection[i].Points.begin(), collection[i].Points.end(),
            shape.Points.begin()))
        {
          currentSeq++;
          maxSeq = std::max(maxSeq, currentSeq);
        }
        else
        {
          currentSeq = 0;
        }
      }
      std::cout << maxSeq << '\n';
    }
    catch (...)
    {
      throw;
    }
  }

  void RECTS(const std::vector< Polygon >& collection, std::string& type)
  {
    (void)type;
    size_t count = std::count_if(collection.begin(), collection.end(), isRectangle);
    std::cout <<  count << '\n';
  }
}


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "no filename\n";
    return -1;
  }
  using timofeev::Polygon;

  std::vector< Polygon > data;
  std::ifstream file(argv[1]);
  std::copy(
    std::istream_iterator< Polygon >(file),
    std::istream_iterator< Polygon >(),
    std::back_inserter(data)
  );

  std::map<std::string, std::function<void(const std::vector< Polygon >&, std::string&)>> commands =
  {
    {"AREA", timofeev::AREA},
    {"MAX", timofeev::MAX},
    {"MIN", timofeev::MIN},
    {"COUNT", timofeev::COUNT},
    {"MAXSEQ", timofeev::MAXSEQ},
    {"RECTS", timofeev::RECTS},
  };

  std::string key;
  while (std::cin >> key)
  {
    try
    {
      std::string arg;
      std::getline(std::cin, arg);
      arg.erase(0, 1);

      auto it = commands.find(key);
      if (it != commands.end()) {
        it->second(data, arg);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
