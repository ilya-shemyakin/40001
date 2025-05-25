#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>


namespace workable
{
    class ShapeError : public std::exception {
    public:
        explicit ShapeError(const std::string& message);
        virtual const char* what() const noexcept override;
    private:
        std::string msg_;
    };

    class DotsError : public std::exception {
    public:
        explicit DotsError(const std::string& message);
        virtual const char* what() const noexcept override;
    private:
        std::string msg_;
    };

    struct Wrapper {
        std::istream& cin;
        std::ostream& cout;
        Wrapper(std::istream& cin, std::ostream& cout) : cin(cin), cout(cout) {};
    };
    struct DelimiterIO {
        char delim;
    };
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

    double distance(const Point& p1, const Point& p2);
    double triangleArea(const Point& p1, const Point& p2, const Point& p3);

    std::istream& operator>>(std::istream& is, DelimiterIO&& delim);
    std::ifstream& operator>>(std::ifstream& is, DelimiterIO&& delim);

    std::vector<std::string> split(const std::string& declar);
};

#endif
