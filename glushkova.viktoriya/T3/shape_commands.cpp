#include "shape_commands.hpp"
#include "stream_guard.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <limits>
#include <string>
#include <numeric>

bool shapes::isEven(const Polygon& poly)
{
    return poly.points.size() % 2 == 0;
}

bool shapes::isOdd(const Polygon& poly)
{
    return poly.points.size() % 2 == 1;
}

void shapes::doArea(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    std::string param;
    if (!(in >> param))
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    if (param == "EVEN")
    {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& poly) {
                return acc + (isEven(poly) ? calcArea(poly) : 0.0);
            });
        out << sum << '\n';
    }
    else if (param == "ODD")
    {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& poly) {
                return acc + (isOdd(poly) ? calcArea(poly) : 0.0);
            });
        out << sum << '\n';
    }
    else if (param == "MEAN")
    {
        if (polygons.empty())
        {
            out << "<INVALID COMMAND>\n";
            return;
        }
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& poly) {
                return acc + calcArea(poly);
            });
        out << total / polygons.size() << '\n';
    }
    else
    {
        try
        {
            size_t target = std::stoul(param);
            if (target < 3)
            {
                out << "<INVALID COMMAND>\n";
                return;
            }
            double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [target](double acc, const Polygon& poly) {
                    return acc + (poly.points.size() == target ? calcArea(poly) : 0.0);
                });
            out << sum << '\n';
        }
        catch (const std::exception&)
        {
            out << "<INVALID COMMAND>\n";
        }
    }
}

void shapes::doMax(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (polygons.empty())
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    std::string param;
    if (!(in >> param))
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    if (param == "AREA")
    {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return calcArea(a) < calcArea(b);
            });
        out << calcArea(*it) << '\n';
    }
    else if (param == "VERTEXES")
    {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
            });
        out << it->points.size() << '\n';
    }
    else
    {
        out << "<INVALID COMMAND>\n";
    }
}

void shapes::doMin(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (polygons.empty())
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    std::string param;
    if (!(in >> param))
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    if (param == "AREA")
    {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return calcArea(a) < calcArea(b);
            });
        out << calcArea(*it) << '\n';
    }
    else if (param == "VERTEXES")
    {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
            });
        out << it->points.size() << '\n';
    }
    else
    {
        out << "<INVALID COMMAND>\n";
    }
}

void shapes::doCount(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    std::string param;
    if (!(in >> param))
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    if (param == "EVEN")
    {
        size_t count = std::count_if(polygons.begin(), polygons.end(), isEven);
        out << count << '\n';
    }
    else if (param == "ODD")
    {
        size_t count = std::count_if(polygons.begin(), polygons.end(), isOdd);
        out << count << '\n';
    }
    else
    {
        try
        {
            size_t target = std::stoul(param);
            if (target < 3)
            {
                out << "<INVALID COMMAND>\n";
                return;
            }
            size_t count = std::count_if(polygons.begin(), polygons.end(),
                [target](const Polygon& poly) {
                    return poly.points.size() == target;
                });
            out << count << '\n';
        }
        catch (const std::exception&)
        {
            out << "<INVALID COMMAND>\n";
        }
    }
}

void shapes::doRmecho(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    size_t removed = 0;
    auto it = polygons.begin();
    while (it != polygons.end())
    {
        if (it != polygons.begin() && *it == target && *it == *(std::prev(it)))
        {
            it = polygons.erase(it);
            removed++;
        }
        else
        {
            ++it;
        }
    }

    out << removed << '\n';
}

void shapes::doSame(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target) || target.points.size() < 3 || in.peek() != '\n')
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    size_t count = std::count_if(polygons.begin(), polygons.end(),
        [&target](const Polygon& poly) {
            return arePolygonsSame(poly, target);
        });

    out << count << '\n';
}
