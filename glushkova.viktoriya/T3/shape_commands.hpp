#ifndef SHAPE_COMMANDS_HPP
#define SHAPE_COMMANDS_HPP
#include "shapes.hpp"
#include <vector>
#include <iostream>

namespace shapes
{
    bool isEven(const Polygon& poly);
    bool isOdd(const Polygon& poly);

    void doArea(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doMax(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doMin(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doCount(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doRmecho(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doSame(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
}
#endif
