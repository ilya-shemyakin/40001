#include "functions.hpp"
#include "functors.hpp"
#include <cstddef>
#include <string>
#include <numeric>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include "commands.hpp"

void AREA(const std::vector<Polygon>& polygons, const std::string& parameter)
{
    if (parameter == "EVEN") {
        double sumOfArea = std::accumulate(polygons.begin(), polygons.end(), 0.0, EvenAccumulator());
        std::cout << std::fixed << std::setprecision(1) << sumOfArea << '\n';
    }
    else if (parameter == "ODD") {
        double sumOfArea = std::accumulate(polygons.begin(), polygons.end(), 0.0, OddAccumulator());
        std::cout << std::fixed << std::setprecision(1) << sumOfArea << '\n';
    }
    else if (parameter == "MEAN") {
        if (polygons.empty()) {
            std::cout << "<INVALID COMMAND>" << '\n';
            return;
        }
        double sumOfArea = std::accumulate(polygons.begin(), polygons.end(), 0.0, MeanAccumulator());
        std::cout << std::fixed << std::setprecision(1) << sumOfArea / polygons.size() << '\n';
    }
    else {
        try {
            size_t num = std::stoul(parameter);
            if (num < 3) {
                throw std::invalid_argument("");
            }
            double sumOfArea = std::accumulate(polygons.begin(), polygons.end(), 0.0, NumOfVertexesAccumulator(num));
            std::cout << std::fixed << std::setprecision(1) << sumOfArea << '\n';
        }
        catch (const std::invalid_argument&) {
            std::cout << "<INVALID COMMAND" << '\n';
        }
    }
}
void MAX(const std::vector<Polygon>& polygons, const std::string& parameter)
{
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << '\n';
        return;
    }
    if (parameter == "AREA") {
        auto maxArea = std::max_element(polygons.begin(), polygons.end(), MaxAreaComparator());
        std::cout << std::fixed << std::setprecision(1) << getArea(*maxArea) << '\n';
    }
    else if (parameter == "VERTEXES") {
        auto maxVertexes = std::max_element(polygons.begin(), polygons.end(), MaxOfVertexesComparator());
        std::cout << std::fixed << std::setprecision(0) << maxVertexes->points.size() << '\n';
    }
    else {
        std::cout << "<INVALID COMMAND>" << '\n';
    }
}
void MIN(const std::vector<Polygon>& polygons, const std::string& parameter)
{
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << '\n';
        return;
    }
    if (parameter == "AREA") {
        auto minArea = std::min_element(polygons.begin(), polygons.end(), MinAreaComparator());
        std::cout << std::fixed << std::setprecision(1) << getArea(*minArea) << '\n';
    }
    else if (parameter == "VERTEXES") {
        auto minVertexes = std::min_element(polygons.begin(), polygons.end(), MinOfVertexesComparator());
        std::cout << std::fixed << std::setprecision(1) << minVertexes->points.size() << '\n';
    }
    else {
        std::cout << "<INVALID COMMAND>" << '\n';
    }
}
void COUNT(const std::vector<Polygon>& polygons, const std::string& parameter)
{
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << '\n';
        return;
    }
    if (parameter == "EVEN") {
        size_t count = std::accumulate(polygons.begin(), polygons.end(), 0, EvenCountAccumulator());
        std::cout << count << '\n';
    }
    else if (parameter == "ODD") {
        size_t count = std::accumulate(polygons.begin(), polygons.end(), 0, OddCountAccumulator());
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
            std::cout << "<INVALID ARGUMENT>" << '\n';
        }
    }
}
void RECTS(const std::vector<Polygon>& polygons, const std::string& parameter)
{
    (void)parameter;
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << '\n';
        return;
    }
    size_t count = std::count_if(polygons.begin(), polygons.end(), IsRectangleChecker());
    std::cout << count << '\n';
}

void INTERSECTIONS(const std::vector<Polygon>& polygons, const std::string& target)
{
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << '\n';
        return;
    }
    std::istringstream iss(target);
    Polygon shape;
    iss >> shape;

    if (!iss || (shape.points.size() < 3)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    size_t count = std::count_if(
        polygons.begin(),
        polygons.end(),
        [&shape](const Polygon& poly) {
            return (poly.points.size() >= 3) && PolygonIntersectionChecker{shape}(poly);
        }
    );
    std::cout << count << '\n';
}
