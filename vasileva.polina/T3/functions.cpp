#include "functions.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <limits>

std::istream& operator>>(std::istream& in, Point& p) {
    char ch;
    if (in >> ch && ch == '(' && in >> p.x >> ch && ch == ';' && in >> p.y >> ch && ch == ')') {
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

void invalComm() {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isEmpty(polys& polys) {
    return polys.size() == 0;
}

double calculateArea(const std::vector<Point>& points) {
    if (points.size() < 3) return 0.0;

    return std::abs(std::accumulate(
        points.begin(), points.end(),
        0.0,
        [&points](double sum, const Point& current) {
            size_t next = (&current - &points[0] + 1) % points.size();
            return sum + (current.x * points[next].y - points[next].x * current.y);
        })) * 0.5;
}

double areaEvenOdd(const std::string& arg, const polys& polys) {
    bool countEven = (arg == "EVEN");

    return std::accumulate(
        polys.begin(),
        polys.end(),
        0.0,
        [countEven](double sum, const Polygon& fig) {
            bool isEven = (fig.points.size() % 2 == 0);
            return sum + ((isEven == countEven) ? calculateArea(fig.points) : 0.0);
        }
    );
}

double areaMean(const polys& polys) {
    return std::accumulate(
        polys.begin(),
        polys.end(),
        0.0,
        [](double sum, const Polygon& fig) {
            return sum + calculateArea(fig.points);
        }
    ) / static_cast<double>(polys.size());

}

double areaNum(size_t arg, const polys& polys) {
    return std::accumulate(
        polys.begin(),
        polys.end(),
        0.0,
        [arg](double sum, const Polygon& fig) {
            if (fig.points.size() == arg) {
                return sum + calculateArea(fig.points);
            }
            return sum;
        }
    );
}

bool VertexCountComparator(const Polygon& a, const Polygon& b) {
    return a.points.size() < b.points.size();
}

bool AreaComparator(const Polygon& a, const Polygon& b) {
    return calculateArea(a.points) < calculateArea(b.points);
}

double maxAreaVer(const std::string& arg, const polys& polys) {
    if (arg == "AREA") {
        auto maxIt = std::max_element(
            polys.begin(),
            polys.end(),
            AreaComparator
        );
        return calculateArea(maxIt->points);
    }
    else if (arg == "VERTEXES") {
        auto maxIt = std::max_element(
            polys.begin(),
            polys.end(),
            VertexCountComparator
        );
        return maxIt->points.size();
    }
    return 0.0;
}

double minAreaVer(const std::string& arg, const polys& polys) {
    if (arg == "AREA") {
        auto minIt = std::min_element(
            polys.begin(),
            polys.end(),
            AreaComparator
        );
        return calculateArea(minIt->points);
    }
    else if (arg == "VERTEXES") {
        auto minIt = std::min_element(
            polys.begin(),
            polys.end(),
            VertexCountComparator
        );
        return minIt->points.size();
    }
    return 0.0;
}

int countVer(const std::string& arg, const polys& polys) {
    return std::count_if(
        polys.begin(),
        polys.end(),
        [&arg](const Polygon& p) {
            bool isEven = p.points.size() % 2 == 0;
            return (arg == "EVEN") ? isEven : !isEven;
        }
    );
}

int countVer(size_t arg, const polys& polys) {
    if (arg < 3) {
        invalComm();
        return -1;
    }
    return std::count_if(
        polys.begin(),
        polys.end(),
        [arg](const Polygon& figure) {
            return figure.points.size() == arg;
        }
    );
}

int echo(polys& data, const Polygon& target) {
    using namespace std::placeholders;

    auto isEqual = std::bind(
        [](const Polygon& a, const Polygon& b) {
            return a.points.size() == b.points.size() &&
                std::equal(a.points.begin(), a.points.end(), b.points.begin(),
                    std::bind(
                        std::logical_and<bool>{},
                        std::bind(
                            std::equal_to<int>{},
                            std::bind(&Point::x, std::placeholders::_1),
                            std::bind(&Point::x, std::placeholders::_2)
                        ),
                        std::bind(
                            std::equal_to<int>{},
                            std::bind(&Point::y, std::placeholders::_1),
                            std::bind(&Point::y, std::placeholders::_2)
                        )
                    )
                );
        },
        std::placeholders::_1,
        std::cref(target)
    );

    int count = std::count_if(data.begin(), data.end(), isEqual);

    std::vector<Polygon> new_data;
    new_data.reserve(data.size() + count);

    std::accumulate(data.begin(), data.end(), std::back_inserter(new_data),
        [&](auto inserter, const Polygon& p) {
            *inserter++ = p;
            if (isEqual(p)) {
                *inserter++ = p;
            }
            return inserter;
        });

    data = std::move(new_data);
    return count;
}

bool hasRightAngle(const std::vector<Point>& figure) {
    if (figure.size() < 3) return false;


    struct AngleChecker {
        const std::vector<Point>& fig;
        size_t i = 0;

        bool operator()(const Point&) {
            const Point& prev = fig[(i + fig.size() - 1) % fig.size()];
            const Point& curr = fig[i];
            const Point& next = fig[(i + 1) % fig.size()];
            i++;

            return (prev.x - curr.x) * (next.x - curr.x)
                + (prev.y - curr.y) * (next.y - curr.y) == 0;
        }
    };

    return std::any_of(figure.begin(), figure.end(), AngleChecker{ figure });
}

int rightShapes(const polys& polys) {
    return std::count_if(polys.begin(), polys.end(),
        [](const Polygon& fig) {
            return hasRightAngle(fig.points);
        });
}
