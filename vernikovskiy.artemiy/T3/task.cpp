#include "task.h"
#include "ScopeGuard.h"
#include <iomanip>

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

//using FunctionVariant = std::variant<
//    std::function<unsigned int(const Wrapper& wrapper)>,
//    std::function<bool(const Wrapper& wrapper)>,
//    std::function<double(const Wrapper& wrapper)>,
//    std::function<void(const Wrapper&)>
//>;

//void invokeFunction(const FunctionVariant& variant_value, const Wrapper& wrapper) {
////    if (std::holds_alternative<std::function<void(const Wrapper&)>>(variant_value)) {
////        auto& func = std::get<std::function<void(const Wrapper&)>>(variant_value);
////        func(wrapper);
////    }
////    else if (std::holds_alternative<std::function<unsigned int(const Wrapper&)>>(variant_value)) {
////        auto& func = std::get<std::function<unsigned int(const Wrapper&)>>(variant_value);
////        unsigned int result = func(wrapper);
////        std::cout << result << std::endl;
////    }
////    else if (std::holds_alternative<std::function<bool(const Wrapper&)>>(variant_value)) {
////        auto& func = std::get<std::function<bool(const Wrapper&)>>(variant_value);
////        bool result = func(wrapper);
////        std::cout << std::boolalpha << result << std::endl;
////    }
////    else if (std::holds_alternative<std::function<double(const Wrapper&)>>(variant_value)) {
////        auto& func = std::get<std::function<double(const Wrapper&)>>(variant_value);
////        double result = func(wrapper);
////        std::cout << result << std::endl;
////    }
//    if (std::holds_alternative<std::function<void(const Wrapper&)>>(variant_value)) {
//        auto& func = std::get<std::function<void(const Wrapper&)>>(variant_value);
//        func(wrapper);
//    }
//}

void Shapes::processCommand(const Wrapper& wrapper) {
    while (!wrapper.cin.eof()) {
        std::string command;
        if (!(wrapper.cin >> command)) {
            //std::cout << ERROR_INVALID_COMMAND << std::endl;
            return;
        }
        if (shapes.size() == 0) {
            std::cout << ERROR_INVALID_COMMAND << std::endl;
            return;
        }
        auto func = functionMap.find(command);
        if (func != functionMap.end()) {
            //auto& func = std::get<std::function<void(const Wrapper&)>>(void);
            func->second(wrapper);
            //invokeFunction(func->second, wrapper);
        } else {
            std::cout << ERROR_INVALID_COMMAND << std::endl;
        }
    }
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
        std::cout << ERROR_INVALID_COMMAND << std::endl;
        std::cout << wrapper.cin.fail() << std::endl;
        if (!wrapper.cin.eof() && !wrapper.cin.bad()) {
            wrapper.cin.clear();
        }
    }

    return Point(x, y);
}

Point Shapes::parsePoint(std::ifstream& ifStream) {
    int x, y;
    if (!(ifStream >> DelimiterIO{'('} >> x >> DelimiterIO{';'} >> y >> DelimiterIO{')'}))
    {
        std::cout << ERROR_INVALID_COMMAND << std::endl;
        std::cout << ifStream.fail() << std::endl;
        if (!ifStream.eof() && !ifStream.bad()) {
            ifStream.clear();
        }
    }

    return Point(x, y);
}

Polygon Shapes::parseShape(std::ifstream& ifStream) {
    int dots = 0;
    if (!(ifStream >> dots)) {
        return Polygon();
    }
    if (dots <= 2) {
        std::cout << ERROR_INVALID_COMMAND << std::endl;
        return Polygon();
    }
    Polygon shape;
    for (int i = 0; i < dots; i++)
    {
        shape.points.push_back(parsePoint(ifStream));
    }
    return shape;
}

Polygon Shapes::parseShape(const Wrapper& wrapper) {
    int dots = 0;
    if (!(wrapper.cin >> dots)) {
        return Polygon();
    }
    if (dots <= 2) {
        std::cout << ERROR_INVALID_COMMAND << std::endl;
        return Polygon();
    }
    Polygon shape;
    for (int i = 0; i < dots; i++)
    {
        shape.points.push_back(parsePoint(wrapper));
    }
    return shape;
}

void Shapes::addShape(std::ifstream& ifStream) {
    while (!ifStream.eof()) {
        Polygon shape = parseShape(ifStream);
        if (shape.points.size() != 0) {
            shapes.push_back(shape);
        }
    }
}

bool Shapes::isShapeExist(const Polygon& shape) {
    return (std::find(shapes.begin(), shapes.end(), shape) != shapes.end());
}

void Shapes::echo(const Wrapper& wrapper) {
    Polygon workShape = parseShape(wrapper);
    if (workShape.points.size() == 0) {
        wrapper.cout << 0 << std::endl;
        return;
    }
    std::vector<decltype(shapes.begin())> tmp;
    for (auto shapeit = shapes.begin(); shapeit != shapes.end(); ++shapeit) { // std::find
        if (workShape == *shapeit) {
            tmp.push_back(shapeit + 1);
        }
    }
    for (auto pos : tmp) {
        shapes.insert(pos, workShape);
    }
    wrapper.cout << tmp.size() << std::endl;
}

void Shapes::inFrame(const Wrapper& wrapper) {
    Polygon workShape = parseShape(wrapper);
    if (workShape.points.size() == 0) {
        wrapper.cout << std::boolalpha << false << std::endl;
        return;
    }
    Polygon rect = buildFrame();
    for (auto& point : workShape.points)
    {
        if (!(rect.points[0] < point && rect.points[1] > point)) {
            wrapper.cout << std::boolalpha << false << std::endl;
            return;
        }
    }
    wrapper.cout << std::boolalpha << true << std::endl;
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
            size_t numVerts = std::stoi(param);
            auto countLambda = [numVerts](double sum, const Polygon& a) {
                return (a.points.size() == numVerts) ? sum + 1 : sum;
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
            wrapper.cout << 0 << std::endl;
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
        size_t numVerts = std::stoi(param);
        auto countLambda = [numVerts](double sum, const Polygon& a) {
            return (a.points.size() == numVerts) ? sum + 1 : sum;
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

}
