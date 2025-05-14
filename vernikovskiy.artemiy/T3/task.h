#ifndef TASK_H
#define TASK_H

#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <map>
#include <variant>
#include <functional>
#include <sstream>
#include "additional.h"

// 5 var
using namespace workable;
namespace doomsday
{
    struct Point
    {
        int x_, y_;
        Point(int x, int y): x_(x), y_(y) {};
        Point(): x_(0), y_(0) {};

        bool operator==(const Point& other) const {
            return x_ == other.x_ && y_ == other.y_;
        }

        bool operator<(const Point& other) const {
            return x_ < other.x_ && y_ < other.y_;
        }

        bool operator>(const Point& other) const {
            return x_ > other.x_ && y_ > other.y_;
        }
    };
    struct Polygon
    {
        std::vector< Point > points;
        bool operator==(const Polygon& other) const {
            return points == other.points;
        }

        bool operator==(std::nullptr_t) const {
            return false;
        }

        Polygon() {};
    };

    class Shapes
    {
    private:
        static std::vector< Polygon > shapes;
        static constexpr std::string_view ERROR_INVALID_COMMAND = "<INVALID COMMAND>";
        using FunctionVariant = std::variant<
            std::function<unsigned int(const Wrapper& wrapper)>,
            std::function<bool(const Wrapper& wrapper)>,
            std::function<double(const Wrapper& wrapper)>,
            std::function<void(const Wrapper&)>
        >;
        std::map<std::string, FunctionVariant> functionMap;

        std::vector< std::string > split(const std::string& str, const char delimiter);
        static Point parsePoint(const Wrapper& wrapper);
        Point parsePoint(const std::string& declar);
        static Polygon buildFrame();
        static double getPolygonArea(const Polygon& shape);


    public:
        Shapes();
        static Polygon parseShape(const Wrapper& wrapper);
        Polygon parseShape(const int dots, const std::vector< std::string >& data);
        void addShape(const std::string& param);

        void processCommand(const Wrapper& wrapper);
        static bool isShapeExist(const Polygon& shape);

        static unsigned int echo(const Wrapper& wrapper);
        static bool inFrame(const Wrapper& wrapper);
        static double area(const Wrapper& wrapper);
        static double max(const Wrapper& wrapper);
        static double min(const Wrapper& wrapper);
        static double count(const Wrapper& wrapper);

        void print();

        ~Shapes() = default;
    };
};

#endif
