#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <numeric>
#include <limits>
#include <functional>
#include <string>
#include <iomanip>
#include "polygon.hpp"
#include "commands.hpp"


int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Not enough arguments\n";
        return 1;
    }
    std::vector< Polygon > vector;
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Fail to open file\n";
        return 1;
    }
    using iter = std::istream_iterator< Polygon >;
    while (!file.eof()) {
        std::copy(iter(file), iter(), std::back_inserter(vector));
        if (file.fail() && !file.eof()) {
            file.clear();
            file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
    }
    static const std::map< std::string, std::function< bool(const std::vector< Polygon >&) > > commands = {
        {"AREA", area},
        {"MAX", maxx},
        {"MIN", minn},
        {"COUNT", count},
        {"RECTS", rects},
        {"INTERSECTIONS", intersections}
    };
    std::cout << std::fixed << std::setprecision(1);
    std::string command;
    auto end = commands.cend();
    while (!(std::cin >> command).eof()) {
        auto current = commands.find(command);
        if ((current == end) || !(current->second)(vector)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
            std::cout << "<INVALID COMMAND>\n";
        }
    }
}
