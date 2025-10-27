#include "commands.hpp"
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>
#include <iomanip>
#include <string>
#include <sstream>
#include "scopeGuard.hpp"

namespace
{
    double calculatePointsProduct(const vladimirova::Point& p1, const vladimirova::Point& p2)
    {
        return p1.x * p2.y - p1.y * p2.x;
    }

    double computePolygonArea(const vladimirova::Polygon& polygon)
    {
        if (polygon.points.size() < 3)
        {
            return 0.0;
        }
        vladimirova::Polygon rotatedPolygon = polygon;
        std::rotate(rotatedPolygon.points.begin(), rotatedPolygon.points.begin() + 1, rotatedPolygon.points.end());
        auto it1 = polygon.points.begin();
        auto it2 = polygon.points.end();
        auto it3 = rotatedPolygon.points.begin();
        double sum = std::inner_product(it1, it2, it3, 0.0, std::plus<double>(), calculatePointsProduct);
        return std::abs(sum) * 0.5;
    }

    size_t getVertexCount(const vladimirova::Polygon& polygon)
    {
        return polygon.points.size();
    }

    bool isEvenVertexCount(const vladimirova::Polygon& polygon)
    {
        return polygon.points.size() % 2 == 0;
    }

    bool isOddVertexCount(const vladimirova::Polygon& polygon)
    {
        return polygon.points.size() % 2 != 0;
    }

    bool hasSpecificVertexCount(const vladimirova::Polygon& polygon, size_t count)
    {
        return polygon.points.size() == count;
    }

    size_t stringToSizeT(const std::string& str)
    {
        std::istringstream iss(str);
        size_t result;
        iss >> result;
        if (iss.fail() || !iss.eof())
        {
            throw std::invalid_argument("Invalid number");
        }
        return result;
    }

    void outputAreaEven(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons)
    {
        std::vector<vladimirova::Polygon> evenPolygons;
        std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(evenPolygons), isEvenVertexCount);
        std::vector<double> areas(evenPolygons.size());
        std::transform(evenPolygons.begin(), evenPolygons.end(), areas.begin(), computePolygonArea);
        double totalArea = std::accumulate(areas.begin(), areas.end(), 0.0);
        iofmtguard fmtguard(out);
        out << std::fixed << std::setprecision(1) << totalArea << '\n';
    }

    void outputAreaOdd(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons)
    {
        std::vector<vladimirova::Polygon> oddPolygons;
        std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(oddPolygons), isOddVertexCount);
        std::vector<double> areas(oddPolygons.size());
        std::transform(oddPolygons.begin(), oddPolygons.end(), areas.begin(), computePolygonArea);
        double totalArea = std::accumulate(areas.begin(), areas.end(), 0.0);
        iofmtguard fmtguard(out);
        out << std::fixed << std::setprecision(1) << totalArea << '\n';
    }

    void outputAreaMean(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons)
    {
        if (polygons.empty())
        {
            throw std::logic_error("No polygons available");
        }
        std::vector<double> areas(polygons.size());
        std::transform(polygons.begin(), polygons.end(), areas.begin(), computePolygonArea);
        double averageArea = std::accumulate(areas.begin(), areas.end(), 0.0) / polygons.size();
        iofmtguard fmtguard(out);
        out << std::fixed << std::setprecision(1) << averageArea << '\n';
    }

    void outputAreaByVertexCount(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons, size_t vertexCount)
    {
        std::vector<vladimirova::Polygon> filteredPolygons;
        auto predicate = std::bind(hasSpecificVertexCount, std::placeholders::_1, vertexCount);
        std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), predicate);
        std::vector<double> areas(filteredPolygons.size());
        std::transform(filteredPolygons.begin(), filteredPolygons.end(), areas.begin(), computePolygonArea);
        double totalArea = std::accumulate(areas.begin(), areas.end(), 0.0);
        iofmtguard fmtguard(out);
        out << std::fixed << std::setprecision(1) << totalArea << '\n';
    }

    void outputMaxArea(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons)
    {
        std::vector<double> areas(polygons.size());
        std::transform(polygons.begin(), polygons.end(), areas.begin(), computePolygonArea);
        iofmtguard fmtguard(out);
        double maxArea = *std::max_element(areas.begin(), areas.end());
        out << std::fixed << std::setprecision(1) << maxArea << '\n';
    }

    void outputMaxVertexCount(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons)
    {
        std::vector<size_t> vertexCounts(polygons.size());
        std::transform(polygons.begin(), polygons.end(), vertexCounts.begin(), getVertexCount);
        iofmtguard fmtguard(out);
        size_t maxVertexCount = *std::max_element(vertexCounts.begin(), vertexCounts.end());
        out << maxVertexCount << '\n';
    }

    void outputMinArea(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons)
    {
        std::vector<double> areas(polygons.size());
        std::transform(polygons.begin(), polygons.end(), areas.begin(), computePolygonArea);
        iofmtguard fmtguard(out);
        double minArea = *std::min_element(areas.begin(), areas.end());
        out << std::fixed << std::setprecision(1) << minArea << '\n';
    }

    void outputMinVertexCount(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons)
    {
        std::vector<size_t> vertexCounts(polygons.size());
        std::transform(polygons.begin(), polygons.end(), vertexCounts.begin(), getVertexCount);
        iofmtguard fmtguard(out);
        size_t minVertexCount = *std::min_element(vertexCounts.begin(), vertexCounts.end());
        out << minVertexCount << '\n';
    }

    void outputCountEven(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons)
    {
        size_t count = std::count_if(polygons.begin(), polygons.end(), isEvenVertexCount);
        out << count << '\n';
    }

    void outputCountOdd(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons)
    {
        size_t count = std::count_if(polygons.begin(), polygons.end(), isOddVertexCount);
        out << count << '\n';
    }

    void outputCountByVertexCount(std::ostream& out, const std::vector<vladimirova::Polygon>& polygons, size_t vertexCount)
    {
        auto predicate = std::bind(hasSpecificVertexCount, std::placeholders::_1, vertexCount);
        size_t count = std::count_if(polygons.begin(), polygons.end(), predicate);
        out << count << '\n';
    }

    void processEchoCommand(std::ostream& out, std::vector<vladimirova::Polygon>& polygons, const vladimirova::Polygon& targetPolygon)
    {
        std::vector<vladimirova::Polygon> modifiedPolygons = polygons;
        size_t duplicatesAdded = 0;
        auto it = std::find(polygons.begin(), polygons.end(), targetPolygon);

        while (it != polygons.end())
        {
            ++duplicatesAdded;
            size_t position = std::distance(polygons.begin(), it) + duplicatesAdded;
            modifiedPolygons.insert(modifiedPolygons.begin() + position, targetPolygon);
            it = std::find(std::next(it), polygons.end(), targetPolygon);
        }

        polygons = std::move(modifiedPolygons);
        out << duplicatesAdded << '\n';
    }

    size_t findMaxConsecutiveSequence(const std::vector<vladimirova::Polygon>& polygons, const vladimirova::Polygon& targetPolygon)
    {
        size_t maxSequence = 0;
        size_t currentSequence = 0;

        for (const auto& polygon : polygons)
        {
            if (polygon == targetPolygon)
            {
                ++currentSequence;
                maxSequence = std::max(maxSequence, currentSequence);
            }
            else
            {
                currentSequence = 0;
            }
        }

        return maxSequence;
    }
}

void vladimirova::processArea(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
{
    std::map<std::string, std::function<void()>> subCommands;
    subCommands["EVEN"] = std::bind(outputAreaEven, std::ref(out), std::cref(polygons));
    subCommands["ODD"] = std::bind(outputAreaOdd, std::ref(out), std::cref(polygons));
    subCommands["MEAN"] = std::bind(outputAreaMean, std::ref(out), std::cref(polygons));

    std::string subCommand;
    in >> subCommand;

    try
    {
        subCommands.at(subCommand)();
    }
    catch (const std::out_of_range&)
    {
        try
        {
            size_t vertexCount = stringToSizeT(subCommand);
            if (vertexCount < 3)
            {
                throw std::logic_error("Invalid vertex count");
            }
            outputAreaByVertexCount(out, polygons, vertexCount);
        }
        catch (const std::invalid_argument&)
        {
            throw std::logic_error("Invalid subcommand");
        }
    }
}

void vladimirova::processMax(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
{
    if (polygons.empty())
    {
        throw std::logic_error("No polygons available");
    }

    std::map<std::string, std::function<void()>> subCommands;
    subCommands["AREA"] = std::bind(outputMaxArea, std::ref(out), std::cref(polygons));
    subCommands["VERTEXES"] = std::bind(outputMaxVertexCount, std::ref(out), std::cref(polygons));

    std::string subCommand;
    in >> subCommand;

    try
    {
        subCommands.at(subCommand)();
    }
    catch (const std::out_of_range&)
    {
        throw std::logic_error("Invalid subcommand");
    }
}

void vladimirova::processMin(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
{
    if (polygons.empty())
    {
        throw std::logic_error("No polygons available");
    }

    std::map<std::string, std::function<void()>> subCommands;
    subCommands["AREA"] = std::bind(outputMinArea, std::ref(out), std::cref(polygons));
    subCommands["VERTEXES"] = std::bind(outputMinVertexCount, std::ref(out), std::cref(polygons));

    std::string subCommand;
    in >> subCommand;

    try
    {
        subCommands.at(subCommand)();
    }
    catch (const std::out_of_range&)
    {
        throw std::logic_error("Invalid subcommand");
    }
}

void vladimirova::processCount(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
{
    std::map<std::string, std::function<void()>> subCommands;
    subCommands["EVEN"] = std::bind(outputCountEven, std::ref(out), std::cref(polygons));
    subCommands["ODD"] = std::bind(outputCountOdd, std::ref(out), std::cref(polygons));

    std::string subCommand;
    in >> subCommand;

    try
    {
        subCommands.at(subCommand)();
    }
    catch (const std::out_of_range&)
    {
        try
        {
            size_t vertexCount = stringToSizeT(subCommand);
            if (vertexCount < 3)
            {
                throw std::logic_error("Invalid vertex count");
            }
            outputCountByVertexCount(out, polygons, vertexCount);
        }
        catch (const std::invalid_argument&)
        {
            throw std::logic_error("Invalid subcommand");
        }
    }
}

void vladimirova::processEcho(std::istream& in, std::ostream& out, std::vector < Polygon& polygons)
{
    if (polygons.empty())
    {
        throw std::logic_error("No polygons available");
    }

    Polygon targetPolygon;
    if (!(in >> targetPolygon))
    {
        throw std::logic_error("Invalid polygon input");
    }

    if (in.peek() != '\n')
    {
        throw std::logic_error("Extra input after polygon");
    }

    processEchoCommand(out, polygons, targetPolygon);
}

void vladimirova::processMaxSequence(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
{
    if (polygons.empty())
    {
        throw std::logic_error("No polygons available");
    }

    Polygon targetPolygon;
    if (!(in >> targetPolygon) || in.peek() != '\n')
    {
        throw std::logic_error("Invalid polygon input");
    }

    size_t maxSequence = findMaxConsecutiveSequence(polygons, targetPolygon);
    out << maxSequence << '\n';
}
