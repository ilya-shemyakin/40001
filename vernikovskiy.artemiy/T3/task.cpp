#include "task.h"
#include "ScopeGuard.h"
#include <iomanip>
#include <vector>
#include <limits>

namespace doomsday
{
std::vector<Polygon> Shapes::shapes;
Shapes::Shapes() {
    functionMap["ECHO"] = std::function<void(const Wrapper&)>(echo);
    functionMap["INFRAME"] = std::function<void(const Wrapper&)>(inFrame);
    functionMap["MIN"] = std::function<void(const Wrapper&)>(min);
    functionMap["MAX"] = std::function<void(const Wrapper&)>(max);
    functionMap["AREA"] = std::function<void(const Wrapper&)>(area);
    functionMap["COUNT"] = std::function<void(const Wrapper&)>(count);
}

void Shapes::print() {
    for (auto& shape : shapes)
    {
        std::cout << shape.points.size() << " ";
        for (auto& point : shape.points)
        {
            std::cout << "(" << point.x_ << ";" << point.y_ << ")" << " ";
        }
        std::cout << std::endl;
    }
}

void Shapes::processCommand(const Wrapper& wrapper) {
    while (!wrapper.cin.eof()) {
        std::string command;
        if (!(wrapper.cin >> command)) {
            return;
        }
        auto func = functionMap.find(command);
        if (func != functionMap.end()) {
            func->second(wrapper);
        } else {
            wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
            wrapper.cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

Point Shapes::parsePoint(std::ifstream& ifStream) {
    int x, y;
    if (!(ifStream >> DelimiterIO{'('} >> x >> DelimiterIO{';'} >> y >> DelimiterIO{')'}))
    {
        if (!ifStream.eof() && !ifStream.bad()) {
            ifStream.clear();
        }
        throw ShapeError(ERROR_INVALID_COMMAND);
    }

    return Point(x, y);
}

Point Shapes::parsePoint(std::string& dot) {
    int x, y;
    std::stringstream ss(dot);
    if (!(ss >> DelimiterIO{'('} >> x >> DelimiterIO{';'} >> y >> DelimiterIO{')'}))
    {
        throw ShapeError(ERROR_INVALID_COMMAND);
    }

    return Point(x, y);
}

Polygon Shapes::parseShape(std::ifstream& ifStream) {
    int dots = 0;
    if (!(ifStream >> dots)) {
        throw DotsError("");
    }
    if (dots <= 2) {
        throw DotsError(ERROR_INVALID_COMMAND);
    }
    Polygon shape;
    std::string dotsSpaces;
    std::getline(ifStream, dotsSpaces);
    std::vector<std::string> tokensDots = split(dotsSpaces);
    if (static_cast<size_t>(dots) != tokensDots.size() - 1) {
        throw ShapeError(ERROR_INVALID_COMMAND);
    }
    for (size_t i = 1; i < tokensDots.size(); i++) {
        shape.points.push_back(parsePoint(tokensDots[i]));
    }
    return shape;
}

Polygon Shapes::buildFrame() {
    int xLeft = 999, xRight = -999, yDown = 999, yUp = -999;
    for (auto& shape : shapes)
    {
        for (auto& point : shape.points)
        {
            xLeft = std::min(xLeft, point.x_);
            xRight = std::max(xRight, point.x_);
            yDown = std::min(yDown, point.y_);
            yUp = std::max(yUp, point.y_);
        }
    }
    Polygon res;
    res.points.push_back(Point(xLeft, yDown));
    res.points.push_back(Point(xRight, yUp));
    return res;
}

Point Shapes::parsePoint(const Wrapper& wrapper) {
    int x, y;
    if (!(wrapper.cin >> DelimiterIO{'('} >> x >> DelimiterIO{';'} >> y >> DelimiterIO{')'}))
    {
        if (!wrapper.cin.eof() && !wrapper.cin.bad()) {
            wrapper.cin.clear();
        }
        throw ShapeError(ERROR_INVALID_COMMAND);
    }

    return Point(x, y);
}

Polygon Shapes::parseShape(const Wrapper& wrapper) {
    int dots = 0;
    if (!(wrapper.cin >> dots)) {
        throw DotsError("");
    }
    if (dots <= 2) {
        throw DotsError(ERROR_INVALID_COMMAND);
    }
    Polygon shape;
    std::string dotsSpaces;
    std::getline(wrapper.cin, dotsSpaces);
    std::vector<std::string> tokensDots = split(dotsSpaces);
    if (static_cast<size_t>(dots) != tokensDots.size() - 1) {
        throw ShapeError(ERROR_INVALID_COMMAND);
    }
    for (size_t i = 1; i < tokensDots.size(); i++) {
        shape.points.push_back(parsePoint(tokensDots[i]));
    }

    return shape;
}

void Shapes::addShape(std::ifstream& ifStream) {
    while (!ifStream.eof()) {
        try {
            Polygon shape = parseShape(ifStream);
            shapes.push_back(shape);
        } catch (const workable::ShapeError& e) {

        } catch (const workable::DotsError& e) {
            ifStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

bool Shapes::isShapeExist(const Polygon& shape) {
    return (std::find(shapes.begin(), shapes.end(), shape) != shapes.end());
}

void Shapes::echo(const Wrapper& wrapper) {
    Polygon workShape;
    try {
        workShape = parseShape(wrapper);
    } catch (const workable::ShapeError& e) {
        wrapper.cout << e.what() << std::endl;
        return;
    } catch (const workable::DotsError& e) {
        wrapper.cout << e.what() << std::endl;
        wrapper.cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    if (workShape.points.size() == 0) {
        return;
    }
    size_t echoCount = 0;
    auto shapeIt = shapes.begin();
    while (shapeIt != shapes.end()) {
        shapeIt = std::find(shapeIt, shapes.end(), workShape);
        if (shapeIt != shapes.end()) {
            shapeIt = shapes.insert(shapeIt + 1, workShape);
            ++shapeIt;
            echoCount++;
        }
    }
    wrapper.cout << echoCount << std::endl;
}

void Shapes::inFrame(const Wrapper& wrapper) {
    Polygon workShape;
    try {
        workShape = parseShape(wrapper);
    } catch (const workable::ShapeError& e) {
        wrapper.cout << e.what() << std::endl;
        return;
    } catch (const workable::DotsError& e) {
        wrapper.cout << e.what() << std::endl;
        wrapper.cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    if (workShape.points.size() == 0) {
        wrapper.cout << "<FALSE>" << std::endl;
        return;
    }
    Polygon rect = buildFrame();
    if (std::count_if(workShape.points.begin(), workShape.points.end(),
                   [&](const auto& point) {
                       return !(rect.points[0] < point && rect.points[1] > point);
                   }) > 0) {
        wrapper.cout << "<FALSE>" << std::endl;
        return;
    }
    wrapper.cout << "<TRUE>" << std::endl;
}

double Shapes::getPolygonArea(const Polygon& shape) {
    if (shape.points.size() < 3) {
        return 0;
    }
    double area = 0.0;
    for (size_t i = 1; i < shape.points.size() - 1; i++) {
        area += (triangleArea(shape.points[0], shape.points[i], shape.points[i+1]));
    }
    return area;
}

void Shapes::area(const Wrapper& wrapper) {
    std::ostream::sentry sentry(wrapper.cout);
    if (sentry) {
        StreamGuard guard(wrapper.cout);
        double res = 0;
        std::string param;
        if (!(wrapper.cin >> param)) {
            wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
            return;
        }

        if (shapes.size() == 0 && param == "MEAN") {
            wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
            wrapper.cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        const std::unordered_map<std::string, std::function<double()>> commandFuncs = {
            {"EVEN", []() {
                auto sumLambda = [](double sum, const Polygon& a) {
                    return (a.points.size() % 2 == 0) ? sum + getPolygonArea(a) : sum;
                };
                return std::accumulate(shapes.begin(), shapes.end(), 0.0, sumLambda);
            }},
            {"ODD", []() {
                auto sumLambda = [](double sum, const Polygon& a) {
                    return (a.points.size() % 2 == 1) ? sum + getPolygonArea(a) : sum;
                };
                return std::accumulate(shapes.begin(), shapes.end(), 0.0, sumLambda);
            }},
            {"MEAN", []() {
                if (shapes.empty()) return 0.0;
                auto sumLambda = [](double sum, const Polygon& a) {
                    return sum + getPolygonArea(a);
                };
                double total = std::accumulate(shapes.begin(), shapes.end(), 0.0, sumLambda);
                return total / shapes.size();
            }}
        };

        try {
            int numVerts = std::stoi(param);
            if (numVerts < 3) {
                wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
                return;
            }
            size_t normalNumVerts = static_cast<size_t>(numVerts);
            auto countLambda = [normalNumVerts](double sum, const Polygon& a) {
                return (a.points.size() == normalNumVerts) ? sum + getPolygonArea(a) : sum;
            };
            res = std::accumulate(shapes.begin(), shapes.end(), 0.0, countLambda);
            wrapper.cout << std::fixed << std::setprecision(1) << res << std::endl;
            return;
        } catch (const std::invalid_argument&) {
            auto it = commandFuncs.find(param);
            if (it != commandFuncs.end()) {
                res = it->second();
                wrapper.cout << std::fixed << std::setprecision(1) << res << std::endl;
                return;
            }
            wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
            return;
        }
    }
}

void Shapes::max(const Wrapper& wrapper) {
    std::ostream::sentry sentry(wrapper.cout);
    if (sentry) {
        StreamGuard guard(wrapper.cout);
        std::string param;
        if (!(wrapper.cin >> param)) {
            wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
            return;
        }

        const std::unordered_map<std::string, std::function<double(const Polygon&)>> extractors = {
            {"AREA", [](const Polygon& p) {
                std::cout << std::fixed << std::setprecision(1);
                return getPolygonArea(p);
            }},
            {"VERTEXES", [](const Polygon& p) { return p.points.size(); }}
        };

        const std::unordered_map<std::string, std::function<bool(const Polygon&, const Polygon&)>> comparators = {
            {"AREA", [](const Polygon& p1, const Polygon& p2) {
                return getPolygonArea(p1) < getPolygonArea(p2);
            }},
            {"VERTEXES", [](const Polygon& p1, const Polygon& p2) {
                return p1.points.size() < p2.points.size();
            }}
        };

        auto compIt = comparators.find(param);
        auto extractIt = extractors.find(param);

        if (compIt == comparators.end() || extractIt == extractors.end()) {
            wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
            return;
        }

        auto comp = compIt->second;
        auto maxIt = std::max_element(shapes.begin(), shapes.end(), comp);
        if (maxIt == shapes.end()) {
            wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
            return;
        }

        wrapper.cout << extractIt->second(*maxIt) << std::endl;
    }
}

void Shapes::min(const Wrapper& wrapper) {
    std::ostream::sentry sentry(wrapper.cout);
    if (sentry) {
        StreamGuard guard(wrapper.cout);
        std::string param;
        if (!(wrapper.cin >> param)) {
            return;
        }

        const std::unordered_map<std::string, std::function<double(const Polygon&)>> extractors = {
            {"AREA", [](const Polygon& p) {
                std::cout << std::fixed << std::setprecision(1);
                return getPolygonArea(p);
            }},
            {"VERTEXES", [](const Polygon& p) { return p.points.size(); }}
        };

        const std::unordered_map<std::string, std::function<bool(const Polygon&, const Polygon&)>> comparators = {
            {"AREA", [](const Polygon& p1, const Polygon& p2) {
                return getPolygonArea(p1) < getPolygonArea(p2);
            }},
            {"VERTEXES", [](const Polygon& p1, const Polygon& p2) {
                return p1.points.size() < p2.points.size();
            }}
        };

        auto compIt = comparators.find(param);
        auto extractIt = extractors.find(param);

        if (compIt == comparators.end() || extractIt == extractors.end()) {
            wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
            return;
        }

        auto comp = compIt->second;
        auto minIt = std::min_element(shapes.begin(), shapes.end(), comp);
        if (minIt == shapes.end()) {
            wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
            return;
        }

        wrapper.cout << std::fixed << std::setprecision(1) << extractIt->second(*minIt) << std::endl;
    }
}

void Shapes::count(const Wrapper& wrapper) {
    double res = 0;
    std::string param;
    if (!(wrapper.cin >> param)) {
        wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
        return;
    }

    const std::unordered_map<std::string, std::function<double()>> commandFuncs = {
        {"EVEN", []() {
            auto sumLambda = [](double sum, const Polygon& a) {
                return (a.points.size() % 2 == 0) ? sum + 1 : sum;
            };
            return std::accumulate(shapes.begin(), shapes.end(), 0.0, sumLambda);
        }},
        {"ODD", []() {
            auto sumLambda = [](double sum, const Polygon& a) {
                return (a.points.size() % 2 == 1) ? sum + 1 : sum;
            };
            return std::accumulate(shapes.begin(), shapes.end(), 0.0, sumLambda);
        }}
    };

    try {
        int numVerts = std::stoi(param);
        if (numVerts < 3) {
            wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
            return;
        }
        size_t normalNumVerts = static_cast<size_t>(numVerts);
        auto countLambda = [normalNumVerts](double sum, const Polygon& a) {
            return (a.points.size() == normalNumVerts) ? sum + 1 : sum;
        };
        res = std::accumulate(shapes.begin(), shapes.end(), 0.0, countLambda);
        wrapper.cout << res << std::endl;
        return;
    } catch (const std::invalid_argument&) {
        auto it = commandFuncs.find(param);
        if (it != commandFuncs.end()) {
            res = it->second();
            wrapper.cout << res << std::endl;
            return;
        }
        wrapper.cout << ERROR_INVALID_COMMAND << std::endl;
        return;
    }
}

}
