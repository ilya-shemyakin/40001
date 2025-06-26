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
#include <unordered_map>
#include "additional.h"

// 5 var
using namespace workable;
namespace doomsday
{
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
        static constexpr const char* ERROR_INVALID_COMMAND = "<INVALID COMMAND>";
        std::map<std::string, std::function<void(const Wrapper& wrapper)>> functionMap;

        static Point parsePoint(const Wrapper& wrapper);
        Point parsePoint(std::ifstream& ifStream);
        static Point parsePoint(std::string& dot);
        static Polygon buildFrame();
        static double getPolygonArea(const Polygon& shape);
        static Polygon parseShape(const Wrapper& wrapper);
        Polygon parseShape(std::ifstream& ifStream);

        static void echo(const Wrapper& wrapper);
        static void inFrame(const Wrapper& wrapper);
        static void area(const Wrapper& wrapper);
        static void max(const Wrapper& wrapper);
        static void min(const Wrapper& wrapper);
        static void count(const Wrapper& wrapper);
        static bool isShapeExist(const Polygon& shape);


    public:
        Shapes();
        void addShape(std::ifstream& ifStream);

        void processCommand(const Wrapper& wrapper);

        void print();

        ~Shapes() = default;
    };
};

#endif
