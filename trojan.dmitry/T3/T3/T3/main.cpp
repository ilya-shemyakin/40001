#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <utility>
#include <limits>
#include <map>
#include <functional>
#include <string>
#include <sstream>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Not enough arguments" << '\n';
        return 1;
    }
    std::vector< Polygon > vector;
    std::ifstream file(argv[1]);
    while (!file.eof()) {
        std::copy(std::istream_iterator< Polygon >(file), std::istream_iterator< Polygon >(), std::back_inserter(vector));
        if (file.fail() && !file.eof()) {
            file.clear();
            file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
    }
    std::map< std::string, std::function< void(const std::vector< Polygon >&,const std::string&) > > cmnds =
    {
        {"AREA", AREA},
        {"MAX", MAX},
        {"MIN", MIN},
        {"COUNT", COUNT},
        {"RECTS", RECTS},
        {"INTERSECTIONS", INTERSECTIONS}
    };
    
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string command, parameter;
        iss >> command;

        if (command.empty()) {
            continue;
        }
        std::getline(iss >> std::ws, parameter);
        auto it = cmnds.find(command);
        if (it != cmnds.end()) {
            //if (it->second(vector, parameter)
            //{
            //    // good
            //    continue;
            //}
            std::cout << "Good\n";
            continue;
        }
        it->second(vector, parameter);
        std::cout << "<INVALID COMAND>\n";
    }
}
