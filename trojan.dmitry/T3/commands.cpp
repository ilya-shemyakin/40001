#include "functions.hpp"
#include "functors.hpp"
#include <cstddef>
#include <string>
#include <numeric>
#include <iomanip>
#include <functional>
#include <algorithm>
#include <sstream>
#include "commands.hpp"

using namespace std::placeholders;
static auto getData = std::bind(std::mem_fn(&Polygon::points), _1);
static auto getDataSize = std::bind(&decltype(Polygon::points)::size, getData);

bool area(const std::vector< Polygon >& polygons)
{
    std::string parameter;
    std::cin >> parameter;
    if (parameter == "EVEN") {
        double sumOfArea = std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(std::plus<>(), _1,
            std::bind(EvenAreaSelect{}, _2)));
        std::cout << sumOfArea << '\n';
    }
    else if (parameter == "ODD") {
        double sumOfArea = std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(std::plus<>(), _1,
            std::bind(OddAreaSelect{}, _2)));
        std::cout << sumOfArea << '\n';
    }
    else if (parameter == "MEAN") {
        if (polygons.empty()) {
            return false;
        }
        static auto meanAccum = std::bind(std::plus<>(), _1, std::bind(getArea, _2));
        double sumOfArea = std::accumulate(polygons.begin(), polygons.end(), 0.0, meanAccum);
        std::cout << sumOfArea / polygons.size() << '\n';
    }
    else {
        try {
            size_t num = std::stoul(parameter);
            if (num < 3) {
                throw std::invalid_argument("");
            }

            double sumOfArea = std::accumulate(polygons.begin(), polygons.end(), 0.0, NumOfVertexesAccumulator(num));
            std::cout << sumOfArea << '\n';
        }
        catch (const std::invalid_argument&) {
            return false;
        }
    }
    return true;
}
bool max(const std::vector< Polygon >& polygons)
{
    std::string parameter;
    std::cin >> parameter;
    if (polygons.empty()) {
        return false;
    }
    if (parameter == "AREA") {
        static auto getMaxArea = std::bind(std::greater<>(), std::bind(getArea, _1), std::bind(getArea, _2));
        auto maxArea = std::max_element(polygons.begin(), polygons.end(), getMaxArea);
        std::cout << getArea(*maxArea) << '\n';
    }
    else if (parameter == "VERTEXES") {
        static auto getMaxSize = std::bind(std::greater<>(), std::bind(getDataSize, _1), std::bind(getDataSize, _2));
        auto maxVertexes = std::max_element(polygons.begin(), polygons.end(), getMaxSize);
        std::cout << maxVertexes->points.size() << '\n';
    }
    else {
        return false;
    }
    return true;
}
bool min(const std::vector< Polygon >& polygons)
{
    std::string parameter;
    std::cin >> parameter;
    if (polygons.empty()) {
        return false;
    }
    if (parameter == "AREA") {
        static auto getMinArea = std::bind(std::less<>(), std::bind(getArea, _1), std::bind(getArea, _2));
        auto minArea = std::min_element(polygons.begin(), polygons.end(), getMinArea);
        std::cout << getArea(*minArea) << '\n';
    }
    else if (parameter == "VERTEXES") {
        static auto getMinSize = std::bind(std::less<>(), std::bind(getDataSize, _1), std::bind(getDataSize, _2));
        auto minVertexes = std::min_element(polygons.begin(), polygons.end(), getMinSize);
        std::cout << minVertexes->points.size() << '\n';
    }
    else {
        return false;
    }
    return true;
}
bool count(const std::vector< Polygon >& polygons)
{
    std::string parameter;
    std::cin >> parameter;
    if (polygons.empty()) {
        return false;
    }
    if (parameter == "EVEN") {
        size_t count = std::accumulate(polygons.begin(), polygons.end(), 0, std::bind(std::plus<>(), _1,
            std::bind(EvenCountSelecter{}, _2)));
        std::cout << count << '\n';
    }
    else if (parameter == "ODD") {
        size_t count = std::accumulate(polygons.begin(), polygons.end(), 0, std::bind(std::plus<>(), _1,
            std::bind(OddCountSelecter{}, _2)));
        std::cout << count << '\n';
    }
    else {
        try {
            size_t num = std::stoul(parameter);
            if (num < 3) {
                throw std::invalid_argument("");
            }
            size_t count = std::accumulate(polygons.begin(), polygons.end(), 0, VertexCountAccumulator(num));
            std::cout << count << '\n';
        }
        catch (const std::invalid_argument&) {
            return false;
        }
    }
    return true;
}
bool rects(const std::vector<Polygon>& polygons)
{
    if (polygons.empty()) {
        return false;
    }
    size_t count = std::count_if(polygons.begin(), polygons.end(), IsRectangleChecker());
    std::cout << count << '\n';
    return true;
}

bool intersections(const std::vector<Polygon>& polygons)
{
    Polygon shape;
    if (!(std::cin >> shape) || (shape.points.size() < 3)) {
        return false;
    }
    static auto hasAtLeast3Points = std::bind(std::greater_equal<>(), std::bind(getDataSize, _1), 3);
    static auto intersectsWithShape = std::bind(PolygonIntersectionChecker{ shape }, _1);
    static auto cond = std::bind(std::logical_and<>(), std::bind(hasAtLeast3Points, _1),
        std::bind(intersectsWithShape, _1));
    size_t count = std::count_if(
        polygons.begin(),
        polygons.end(),
        std::bind(cond, _1)
    );
    std::cout << count << '\n';
    return true;
}
