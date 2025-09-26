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
#include "polygon.h"

using namespace std::placeholders;

static auto getPolygonPoints = std::mem_fn(&Polygon::points);
static auto getPointsSize = std::bind(&std::vector<Point>::size, std::bind(getPolygonPoints, _1));
static auto getArea = [](const Polygon& poly) { return calculateArea(poly.points); };

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

    if (!(in >> count) || (count < 3)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (int i = 0; i < count; ++i) {
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

    if (in.peek() != EOF && in.peek() != '\n') {
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

    double sum = std::accumulate(indices.begin(), indices.end(), 0.0, op);
    return std::abs(sum) / 2.0;
}

struct EvenAreaSelector {
    double operator()(const Polygon& poly) const {
        return (poly.points.size() % 2 == 0) ? calculateArea(poly.points) : 0.0;
    }
};

struct OddAreaSelector {
    double operator()(const Polygon& poly) const {
        return (poly.points.size() % 2 != 0) ? calculateArea(poly.points) : 0.0;
    }
};

double areaEvenOdd(const std::string& parametr, const std::vector<Polygon>& polygons)
{
    bool checkEven = (parametr == "EVEN");

    auto accumulator = [checkEven](double sum, const Polygon& poly) {
        bool isEven = (getPointsSize(poly) % 2 == 0);
        if (isEven == checkEven) {
            return sum + getArea(poly);
        }
        return sum;
        };

    return std::accumulate(polygons.begin(), polygons.end(), 0.0, accumulator);
}

double areaMean(const std::vector<Polygon>& polys)
{
    if (polys.empty()) return 0.0;

    double result = std::accumulate(polys.begin(), polys.end(), 0.0,
        std::bind(std::plus<>(), _1, std::bind(getArea, _2)));

    return result / static_cast<double>(polys.size());
}

struct VertexAreaSelector {
    size_t vertex;
    VertexAreaSelector(size_t v) : vertex(v) {}

    double operator()(const Polygon& poly) const {
        return (poly.points.size() == vertex) ? calculateArea(poly.points) : 0.0;
    }
};

double areaNumOfVertex(size_t vertex, const std::vector<Polygon>& polys)
{
    double result = std::accumulate(polys.begin(), polys.end(), 0.0,std::bind(std::plus<>(), _1, std::bind(VertexAreaSelector(vertex), _2)));

    return result;
}

struct AreaComparator {
    bool operator()(const Polygon& a, const Polygon& b) const {
        return calculateArea(a.points) < calculateArea(b.points);
    }
};

struct VertexCountComparator {
    bool operator()(const Polygon& a, const Polygon& b) const {
        return a.points.size() < b.points.size();
    }
};

double maxAreaVertexes(const std::string& parametr, const std::vector<Polygon>& polys)
{
    if (polys.empty()) {
        invalidCommand();
        return 0.0;
    }

    if (parametr == "AREA") {
        auto max = std::max_element(polys.begin(), polys.end(), AreaComparator{});
        return calculateArea(max->points);
    }
    else if (parametr == "VERTEXES") {
        auto max = std::max_element(polys.begin(), polys.end(), VertexCountComparator{});
        return static_cast<double>(max->points.size());
    }

    invalidCommand();
    return 0.0;
}

double minAreaVertexes(const std::string& parametr, const std::vector<Polygon>& polys)
{
    if (polys.empty()) {
        invalidCommand();
        return 0.0;
    }

    if (parametr == "AREA") {
        auto min = std::min_element(polys.begin(), polys.end(), AreaComparator{});
        return calculateArea(min->points);
    }
    else if (parametr == "VERTEXES") {
        auto min = std::min_element(polys.begin(), polys.end(), VertexCountComparator{});
        return static_cast<double>(min->points.size());
    }

    invalidCommand();
    return 0.0;
}

struct EvenCountSelector {
    bool operator()(const Polygon& poly) const {
        return poly.points.size() % 2 == 0;
    }
};

struct OddCountSelector {
    bool operator()(const Polygon& poly) const {
        return poly.points.size() % 2 != 0;
    }
};

struct VertexCountSelector {
    size_t vertex;
    VertexCountSelector(size_t v) : vertex(v) {}

    bool operator()(const Polygon& poly) const {
        return poly.points.size() == vertex;
    }
};

size_t countEvenOdd(const std::string& parametr, const std::vector<Polygon>& polys)
{
    bool checkEven = (parametr == "EVEN");

    auto selector = [checkEven](const Polygon& poly) {
        bool isEven = (getPointsSize(poly) % 2 == 0);
        return isEven == checkEven;
        };

    return std::count_if(polys.begin(), polys.end(), selector);
}

int countNumOfVertexes(int vertexes, const std::vector<Polygon>& polys)
{
    if (vertexes < 3) {
        invalidCommand();
        return -1;
    }

    return std::count_if(polys.begin(), polys.end(), std::bind(VertexCountSelector(vertexes), _1));
}

auto getBoundingBox(const std::vector<Polygon>& polys, int& minX, int& maxX, int& minY, int& maxY)
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
        return 0;
        };

    std::for_each(polys.begin(), polys.end(),
        [&](const Polygon& poly) {
            std::for_each(poly.points.begin(), poly.points.end(), updateBounds);
        });
}

bool inFrame(const std::vector<Polygon>& polys, const Polygon& target)
{
    if (target.points.size() < 3) {
        return false;
    }

    if (polys.empty()) {
        return false;
    }

    int min_x, min_y, max_x, max_y;
    getBoundingBox(polys, min_x, max_x, min_y, max_y);

    auto isInFrame = std::bind(PointInFrameChecker(min_x, min_y, max_x, max_y), _1);

    bool allInFrame = std::all_of(target.points.begin(), target.points.end(), isInFrame);
    return allInFrame;
}

bool arePolygonsEqual(const Polygon& a, const Polygon& b)
{
    if (a.points.size() != b.points.size()) return false;

    std::vector<Point> aPoints = a.points;
    std::vector<Point> bPoints = b.points;

    auto pointComparator = [](const Point& p1, const Point& p2) {
        if (p1.x != p2.x) return p1.x < p2.x;
        return p1.y < p2.y;
        };

    std::sort(aPoints.begin(), aPoints.end(), pointComparator);
    std::sort(bPoints.begin(), bPoints.end(), pointComparator);

    return aPoints == bPoints;
}

size_t removeEcho(std::vector<Polygon>& polys, const Polygon& target)
{
    if (polys.empty()) return 0;

    size_t removedCount = 0;
    auto it = polys.begin();

    while (it != polys.end()) {
        while (it != polys.end() && !arePolygonsEqual(*it, target)) {
            ++it;
        }

        if (it == polys.end()) break;

        auto sequenceStart = it;
        while (it != polys.end() && arePolygonsEqual(*it, target)) {
            ++it;
        }

        size_t sequenceLength = std::distance(sequenceStart, it);
        if (sequenceLength > 1) {
            auto eraseStart = std::next(sequenceStart);
            auto eraseEnd = it;
            removedCount += std::distance(eraseStart, eraseEnd);
            polys.erase(eraseStart, eraseEnd);
            it = sequenceStart;
            ++it;
        }
    }

    return removedCount;
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
{    std::string command;
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
                    auto max = std::max_element(polygons.begin(), polygons.end(), VertexCountComparator{});
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
                    auto min = std::min_element(polygons.begin(), polygons.end(), VertexCountComparator{});
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
                if (!(std::cin >> target) || target.points.size() < 3)
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
            else if (command == "RMECHO")
            {
                Polygon target;
                if (!(std::cin >> target) || target.points.size() < 3)
                {
                    invalidCommand();
                    continue;
                }

                std::vector<Polygon> polysCopy = polygons;
                size_t removed = removeEcho(polysCopy, target);
                std::cout << removed << '\n';
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

