#include "task.h"

namespace doomsday
{
std::vector<Polygon> Shapes::shapes;
Shapes::Shapes() {
    functionMap["ECHO"] = std::function<unsigned int(const Wrapper&)>(echo);
    functionMap["INFRAME"] = std::function<bool(const Wrapper&)>(inFrame);
    functionMap["MIN"] = std::function<double(const Wrapper&)>(min);
    functionMap["MAX"] = std::function<double(const Wrapper&)>(max);
    functionMap["AREA"] = std::function<double(const Wrapper&)>(area);
    functionMap["COUNT"] = std::function<double(const Wrapper&)>(count);
}

std::vector< std::string > Shapes::split(const std::string& str, const char delimiter) {
    std::vector< std::string > tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

using FunctionVariant = std::variant<
    std::function<unsigned int(const Wrapper& wrapper)>,
    std::function<bool(const Wrapper& wrapper)>,
    std::function<double(const Wrapper& wrapper)>,
    std::function<void(const Wrapper&)>
>;

void invokeFunction(const FunctionVariant& variant_value, const Wrapper& wrapper) {
    if (std::holds_alternative<std::function<void(const Wrapper&)>>(variant_value)) {
        auto& func = std::get<std::function<void(const Wrapper&)>>(variant_value);
        func(wrapper);
    }
    else if (std::holds_alternative<std::function<unsigned int(const Wrapper&)>>(variant_value)) {
        auto& func = std::get<std::function<unsigned int(const Wrapper&)>>(variant_value);
        unsigned int result = func(wrapper);
        std::cout << result << std::endl;
    }
    else if (std::holds_alternative<std::function<bool(const Wrapper&)>>(variant_value)) {
        auto& func = std::get<std::function<bool(const Wrapper&)>>(variant_value);
        bool result = func(wrapper);
        std::cout << std::boolalpha << result << std::endl;
    }
    else if (std::holds_alternative<std::function<double(const Wrapper&)>>(variant_value)) {
        auto& func = std::get<std::function<double(const Wrapper&)>>(variant_value);
        double result = func(wrapper);
        std::cout << result << std::endl;
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
            invokeFunction(func->second, wrapper);
        } else {
            std::cout << ERROR_INVALID_COMMAND << std::endl;
        }
    }
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

Point Shapes::parsePoint(const std::string& declar) {
    std::vector< std::string > coords = split(declar.substr(1, declar.size()-1), ';');
    int x, y;
    try {
        x = std::stoi(coords[0]);
        y = std::stoi(coords[1]);
    } catch (const std::invalid_argument& e) {
        std::cout << ERROR_INVALID_COMMAND << std::endl;
        throw std::runtime_error("dummy"); // do my exception with try catch
    }

    return Point(x, y);
}

Polygon Shapes::parseShape(const int dots, const std::vector< std::string >& data) {
    Polygon shape;
    for (int i = 0; i < dots; i++)
    {
        shape.points.push_back(parsePoint(data[i+1]));
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

void Shapes::addShape(const std::string& declar) {
    std::vector< std::string > data = split(declar, ' ');
    int dots = 0;
    try {
        dots = std::stoi(data[0]);
    } catch (const std::invalid_argument& e) {
        // std::cout << ERROR_INVALID_COMMAND << std::endl;
        return;
    }
    if (dots <= 1)
    {
        return;
    }

    shapes.push_back(parseShape(dots, data));
}

bool Shapes::isShapeExist(const Polygon& shape) {
    return (std::find(shapes.begin(), shapes.end(), shape) != shapes.end());
}

unsigned int Shapes::echo(const Wrapper& wrapper) {
    Polygon workShape = parseShape(wrapper);
    if (workShape.points.size() == 0) {
        return 0;
    }
    std::vector<decltype(shapes.begin())> tmp;
    for (auto shapeit = shapes.begin(); shapeit != shapes.end(); ++shapeit) {
        if (workShape == *shapeit) {
            tmp.push_back(shapeit + 1);
        }
    }
    for (auto pos : tmp) {
        shapes.insert(pos, workShape);
    }
    return tmp.size();
}

bool Shapes::inFrame(const Wrapper& wrapper) {
    Polygon workShape = parseShape(wrapper);
    if (workShape.points.size() == 0) {
        return false;
    }
    Polygon rect = buildFrame();
    for (auto& point : workShape.points)
    {
        if (!(rect.points[0] < point && rect.points[1] > point)) {
            return false;
        }
    }
    return true;
}

double Shapes::getPolygonArea(const Polygon& shape) {
    double area = 0.0;
    int n = shape.points.size();
    int j = n - 1;
    for (int i = 0; i < n; i++){
        area += (shape.points[j].x_ - shape.points[i].x_) * 
            (shape.points[j].y_ + shape.points[i].y_); // or vice versa +-
        j = i;
    }
    std::cout << "AREA " << area << std::endl;
    return abs(area / 2.0);
}

double Shapes::area(const Wrapper& wrapper) {
    double res = 0;
    std::string param;
    if (!(wrapper.cin >> param)) {
        return 0;
    }

    const std::unordered_map<std::string, std::function<double()>> commandFuncs = {
        {"EVEN", []() {
            auto sumLambda = [](double sum, const Polygon& a) {
                return (a.points.size() % 2 == 1) ? sum + getPolygonArea(a) : sum;
            };
            return std::accumulate(shapes.begin(), shapes.end(), 0.0, sumLambda);
        }},
        {"ODD", []() {
            auto sumLambda = [](double sum, const Polygon& a) {
                return (a.points.size() % 2 == 0) ? sum + getPolygonArea(a) : sum;
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
        auto countLambda = [numVerts](double sum, const Polygon& a) {
            return (a.points.size() == numVerts) ? sum + 1 : sum;
        };
        res = std::accumulate(shapes.begin(), shapes.end(), 0.0, countLambda);
        return res;
    } catch (const std::invalid_argument&) {
        auto it = commandFuncs.find(param);
        if (it != commandFuncs.end()) {
            res = it->second();
            return res;
        }
        return 0;
    }
}

double Shapes::max(const Wrapper& wrapper) {
    std::string param;
    if (!(wrapper.cin >> param)) {
        return 0;
    }

    const std::unordered_map<std::string, std::function<double(const Polygon&)>> extractors = {
        {"AREA", [](const Polygon& p) { return getPolygonArea(p); }},
        {"VERTEXES", [](const Polygon& p) { return static_cast<double>(p.points.size()); }}
    };

    const std::unordered_map<std::string, std::function<bool(const Polygon&, const Polygon&)>> comparators = {
        {"AREA", [](const Polygon& p1, const Polygon& p2) {
            return getPolygonArea(p1) < getPolygonArea(p2);
        }},
        {"VERTEXES", [](const Polygon& p1, const Polygon& p2) {
            return p1.points.size() < p2.points.size();
        }}
    };

    auto comp_it = comparators.find(param);
    auto extract_it = extractors.find(param);

    if (comp_it == comparators.end() || extract_it == extractors.end()) {
        return 0;
    }

    auto comp = comp_it->second;
    auto maxIt = std::max_element(shapes.begin(), shapes.end(), comp);
    if (maxIt == shapes.end()) {
        return 0;
    }

    return extract_it->second(*maxIt);
}

double Shapes::min(const Wrapper& wrapper) {
    std::string param;
    if (!(wrapper.cin >> param)) {
        return 0;
    }

    const std::unordered_map<std::string, std::function<double(const Polygon&)>> extractors = {
        {"AREA", [](const Polygon& p) { return getPolygonArea(p); }},
        {"VERTEXES", [](const Polygon& p) { return static_cast<double>(p.points.size()); }}
    };

    const std::unordered_map<std::string, std::function<bool(const Polygon&, const Polygon&)>> comparators = {
        {"AREA", [](const Polygon& p1, const Polygon& p2) {
            return getPolygonArea(p1) > getPolygonArea(p2);
        }},
        {"VERTEXES", [](const Polygon& p1, const Polygon& p2) {
            return p1.points.size() > p2.points.size();
        }}
    };

    auto comp_it = comparators.find(param);
    auto extract_it = extractors.find(param);

    if (comp_it == comparators.end() || extract_it == extractors.end()) {
        return 0;
    }

    auto comp = comp_it->second;
    auto minIt = std::min_element(shapes.begin(), shapes.end(), comp);
    if (minIt == shapes.end()) {
        return 0;
    }

    return extract_it->second(*minIt);
}

double Shapes::count(const Wrapper& wrapper) {
    double res = 0;
    std::string param;
    if (!(wrapper.cin >> param)) {
        return 0;
    }

    const std::unordered_map<std::string, std::function<double()>> commandFuncs = {
        {"EVEN", []() {
            auto sumLambda = [](double sum, const Polygon& a) {
                return (a.points.size() % 2 == 1) ? sum + 1 : sum;
            };
            return std::accumulate(shapes.begin(), shapes.end(), 0.0, sumLambda);
        }},
        {"ODD", []() {
            auto sumLambda = [](double sum, const Polygon& a) {
                return (a.points.size() % 2 == 0) ? sum + 1 : sum;
            };
            return std::accumulate(shapes.begin(), shapes.end(), 0.0, sumLambda);
        }}
    };

    try {
        int numVerts = std::stoi(param);
        auto countLambda = [numVerts](double sum, const Polygon& a) {
            return (a.points.size() == numVerts) ? sum + 1 : sum;
        };
        res = std::accumulate(shapes.begin(), shapes.end(), 0.0, countLambda);
        return res;
    } catch (const std::invalid_argument&) {
        auto it = commandFuncs.find(param);
        if (it != commandFuncs.end()) {
            res = it->second();
            return res;
        }
        return 0;
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
