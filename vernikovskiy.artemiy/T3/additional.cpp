#include "additional.h"

namespace workable
{
    std::istream& operator>>(std::istream& is, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(is);
        if (!sentry)
        {
            return is;
            std::cout << "SENTRY" << std::endl;
        }
        char c = '0';
        is >> c;
        if (is && (c != dest.delim))
        {
            is.setstate(std::ios::failbit);
        }
        return is;
    }

    std::ifstream& operator>>(std::ifstream& is, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(is);
        if (!sentry)
        {
            return is;
            std::cout << "SENTRY" << std::endl;
        }
        char c = '0';
        is >> c;
        if (is.good() && (c != dest.delim))
        {
            is.setstate(std::ios::failbit);
        }
        return is;
    }
    
    double distance(const Point& p1, const Point& p2) {
        return std::sqrt((p2.x_ - p1.x_)*(p2.x_ - p1.x_) + (p2.y_ - p1.y_)*(p2.y_ - p1.y_));
    }
    
    double triangleArea(const Point& p1, const Point& p2, const Point& p3) {
        double a = distance(p2, p3);
        double b = distance(p1, p3);
        double c = distance(p1, p2);

        double s = (a + b + c) / 2.0;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }
}
