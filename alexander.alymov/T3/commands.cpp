#include "commands.h"
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cmath>
#include <functional>

namespace alymov
{

    using namespace std::placeholders;

    double calculateArea(const Polygon& poly)
    {
        const auto& points = poly.points;
        int n = points.size();

        auto area = std::abs(std::accumulate(
            points.begin(), points.end(),
            0.0,
            [&points, n](double acc, const Point& p)
            {
                int i = &p - &points[0];
                int j = (i + 1) % n;
                return acc + (p.x * points[j].y) - (points[j].x * p.y);
            }
        )) / 2.0;

        return area;
    }

    void area(std::vector<Polygon>& polygons, std::string& mode, std::ostream& out)
    {
        StreamGuard guard(out);
        if (mode == "EVEN" || mode == "ODD")
        {
            bool even = (mode == "EVEN");
            double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [even](double acc, const Polygon& poly)
                {
                    bool isEven = (poly.points.size() % 2 == 0);
                    return acc + ((isEven == even) ? calculateArea(poly) : 0.0);
                });

            out << std::fixed << std::setprecision(1) << sum << std::endl;
        }
        else if (mode == "MEAN")
        {
            if (polygons.empty())
            {
                out << "<INVALID COMMAND>" << std::endl;
                return;
            }

            double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [](double acc, const Polygon& poly)
                {
                    return acc + calculateArea(poly);
                });

            out << std::fixed << std::setprecision(1) << sum / polygons.size() << std::endl;
        }
        else
        {
            try
            {
                size_t numVertices = std::stoul(mode);
                if (numVertices <= 2)
                {
                    out << "<INVALID COMMAND>" << std::endl;
                    return;
                }
                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                    [numVertices](double acc, const Polygon& poly)
                    {
                        return acc + ((poly.points.size() == numVertices) ? calculateArea(poly) : 0.0);
                    });

                out << std::fixed << std::setprecision(1) << sum << std::endl;
            }
            catch (...)
            {
                out << "<INVALID COMMAND>" << std::endl;
            }
        }
    }

    void max(std::vector<Polygon>& polygons, std::string& mode, std::ostream& out)
    {
        StreamGuard guard(out);
        if (polygons.empty())
        {
            out << "<INVALID COMMAND>" << std::endl;
            return;
        }

        if (mode == "AREA")
        {
            auto it = std::max_element(polygons.begin(), polygons.end(),
                [](const Polygon& a, const Polygon& b) {
                    return calculateArea(a) < calculateArea(b);
                });


            out << std::fixed << std::setprecision(1) << calculateArea(*it) << std::endl;
        }
        else if (mode == "VERTEXES")
        {
            auto it = std::max_element(polygons.begin(), polygons.end(),
                [](const Polygon& a, const Polygon& b) {
                    return a.points.size() < b.points.size();
                });


            out << it->points.size() << std::endl;
        }
        else
        {
            out << "<INVALID COMMAND>" << std::endl;
        }
    }

    void min(std::vector<Polygon>& polygons, std::string& mode, std::ostream& out)
    {
        StreamGuard guard(out);
        if (polygons.empty())
        {
            out << "<INVALID COMMAND>" << std::endl;
            return;
        }

        if (mode == "AREA")
        {

            auto it = std::min_element(polygons.begin(), polygons.end(),
                std::bind(std::less<double>(),
                    std::bind(calculateArea, _1),
                    std::bind(calculateArea, _2)));

            out << std::fixed << std::setprecision(1) << calculateArea(*it) << std::endl;
        }
        else if (mode == "VERTEXES")
        {

            auto it = std::min_element(polygons.begin(), polygons.end(), std::bind(
                std::less<size_t>(),
                std::bind(&std::vector<Point>::size, std::bind(&Polygon::points, _1)),
                std::bind(&std::vector<Point>::size, std::bind(&Polygon::points, _2))
            ));

            out << it->points.size() << std::endl;
        }
        else
        {
            out << "<INVALID COMMAND>" << std::endl;
        }
    }

    void count(std::vector<Polygon>& polygons, std::string& mode, std::ostream& out)
    {
        if (mode == "EVEN" || mode == "ODD")
        {
            bool even = (mode == "EVEN");
            size_t count = std::count_if(polygons.begin(), polygons.end(),
                [even](const Polygon& poly)
                {
                    return (poly.points.size() % 2 == 0) == even;
                });

            out << count << std::endl;
        }
        else
        {
            try
            {
                size_t numVertices = std::stoul(mode);
                if (numVertices <= 2)
                {
                    out << "<INVALID COMMAND>" << std::endl;
                    return;
                }
                size_t count = std::count_if(polygons.begin(), polygons.end(),
                    [numVertices](const Polygon& poly)
                    {
                        return poly.points.size() == numVertices;
                    });

                out << count << std::endl;
            }
            catch (...)
            {
                out << "<INVALID COMMAND>" << std::endl;
            }
        }
    }

    void lessarea(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
    {
        Polygon target;
        in >> target;
        if (!in || target.points.size() < 3)
        {
            out << "<INVALID COMMAND>" << std::endl;
            return;
        }

        double targetArea = calculateArea(target);
        size_t count = std::count_if(polygons.begin(), polygons.end(),
            [targetArea](const Polygon& poly)
            {
                return calculateArea(poly) < targetArea;
            });

        out << count << std::endl;
    }

    void maxseq(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
    {
        Polygon target;
        in >> target;
        if (!in || target.points.size() < 3)
        {
            out << "<INVALID COMMAND>\n";
            return;
        }

        if (polygons.empty())
        {
            out << "0\n";
            return;
        }

        auto polygonsEqual = [](const Polygon& a, const Polygon& b)
            {
                if (a.points.size() != b.points.size()) return false;
                return std::equal(a.points.begin(), a.points.end(), b.points.begin(),
                    [](const Point& p1, const Point& p2) {
                        return p1.x == p2.x && p1.y == p2.y;
                    });
            };

        auto result = std::accumulate(polygons.begin(), polygons.end(),
            std::make_pair(0u, 0u),
            [&target, &polygonsEqual](std::pair<size_t, size_t> acc, const Polygon& poly)
            {
                if (polygonsEqual(poly, target)) {
                    acc.first++;
                    acc.second = std::max(acc.second, acc.first);
                }
                else {
                    acc.first = 0;
                }
                return acc;
            });

        out << result.second << "\n";
    }

}
