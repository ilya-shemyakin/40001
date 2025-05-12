#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

// 5 var
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
    };

    class Shapes
    {
    private:
        std::vector< Polygon > shapes;
        const char* ERROR_INVALID_COMMAND = "<INVALID COMMAND>";
        const std::vector< std::string > valid_commands = {"ECHO", "INFRAME"};

        std::vector< std::string > split(const std::string& str, const char delimiter);
        Point parsePoint(const std::string& declar);
        Polygon buildFrame();

    public:
        Shapes() {};
        Polygon parseShape(const int dots, const std::vector< std::string >& data);
        void addShape(const std::string& declar);

        bool validCommand(const std::string& com);

        void processCommand(const std::string& com);

        bool isShapeExist(const Polygon& shape);

        void echo(const Polygon& workShape);

        bool inFrame(const Polygon& workShape);

        void print();

        ~Shapes() = default;
    };
};
