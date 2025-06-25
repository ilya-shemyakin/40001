#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

namespace alymov
{

    struct Point
    {
        int x;
        int y;
        bool operator ==(const Point& other) const
        {
            return ((other.x == this->x) && (other.y == this->y));
        }
    };

    struct Polygon
    {
        std::vector< Point > points;

        bool operator==(const Polygon& other) const
        {
            if (this->points.size() != other.points.size())
            {
                return false;
            }
            for (size_t i = 0; i < this->points.size(); i++)
            {
                if (!(this->points[i] == other.points[i]))
                {
                    return false;
                }
            }
            return true;
        }
    };

    struct DelimiterIO
    {
        char exp;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, Point& dest);
    std::istream& operator>>(std::istream& in, Polygon& dest);

}
