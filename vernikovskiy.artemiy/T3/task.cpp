#include "task.h"

using namespace doomsday;

std::vector< std::string > Shapes::split(const std::string& str, const char delimiter) {
    std::vector< std::string > tokens;
    std::stringstream ss(str);
    std::string token;


    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
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

Polygon Shapes::parseShape(const int dots, const std::vector< std::string >& data) {
    Polygon shape;
    for (int i = 0; i < dots; i++)
    {
        shape.points.push_back(parsePoint(data[i+1]));
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

bool Shapes::validCommand(const std::string& com) {
    return (std::find(valid_commands.begin(), valid_commands.end(), com) != valid_commands.end());
}

void Shapes::processCommand(const std::string& com) {
    std::vector< std::string > commandTokens = split(com, ' ');
    if (!(validCommand(commandTokens[0]))) {
        std::cout << ERROR_INVALID_COMMAND << std::endl;
        return;
    }
    int dots = 0;
    try {
        dots = std::stoi(commandTokens[1]);
    } catch (const std::invalid_argument& e) {
        std::cout << ERROR_INVALID_COMMAND << std::endl;
        return;
    }

    if (dots <= 2) {
        std::cout << ERROR_INVALID_COMMAND << std::endl;
        return;
    }

    Polygon shape = parseShape(
        dots,
        std::vector< std::string >(commandTokens.begin() + 1, commandTokens.end())
    );

    if (commandTokens[0] == "ECHO") {
        try {
            if (!isShapeExist(shape)) {
                std::cout << "bad\n";
                return;
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return;
        }
        echo(shape);
    } else if (commandTokens[0] == "INFRAME") {
        std::cout << std::boolalpha << inFrame(shape) << std::endl;
    }
}

bool Shapes::isShapeExist(const Polygon& shape) {
    return (std::find(shapes.begin(), shapes.end(), shape) != shapes.end());
}

void Shapes::echo(const Polygon& workShape) {
    std::vector<decltype(shapes.begin())> tmp;
    for (auto shapeit = shapes.begin(); shapeit != shapes.end(); ++shapeit) {
        if (workShape == *shapeit) {
            tmp.push_back(shapeit + 1);
        }
    }
    for (auto pos : tmp) {
        shapes.insert(pos, workShape);
    }
    std::cout << tmp.size() << std::endl;
}

bool Shapes::inFrame(const Polygon& workShape) {
    Polygon rect = buildFrame();
    for (auto& point : workShape.points)
    {
        if (!(rect.points[0] < point && rect.points[1] > point)) {
            return false;
        }
    }
    return true;
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
