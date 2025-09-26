#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <iterator>
#include "functions.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "ERROR: incorrect input\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "ERROR: cannot open file\n";
        return 1;
    }

    polys polygons;

    while (!input.eof()) {
        std::copy(
            std::istream_iterator<Polygon>(input),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygons)
        );

        if (input.fail() && !input.eof()) {
            input.clear();
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::string command;
    std::cout << std::fixed << std::setprecision(1);
    while (std::cin >> command) {
        std::cin.clear();
        if (command == "AREA") {
            std::string arg;
            std::cin >> arg;
            if (arg == "ODD" || arg == "EVEN") {
                std::cout << areaEvenOdd(arg, polygons) << '\n';
            }
            else if (arg == "MEAN") {
                if (isEmpty(polygons)) {
                    invalComm();
                    continue;
                }
                std::cout << areaMean(polygons) << '\n';
            }
            else {
                try {
                    if (std::stoi(arg) >= 3) {
                        std::cout << areaNum(std::stoi(arg), polygons) << '\n';
                    }
                    else {
                        invalComm();
                    }
                }
                catch (...) {
                    invalComm();
                }
            }
        }
        else if (command == "MAX" || command == "MIN") {
            if (isEmpty(polygons)) {
                invalComm();
                continue;
            }
            std::string arg;
            std::cin >> arg;
            if (command == "MAX" && arg == "AREA") {
                std::cout << maxAreaVer(arg, polygons) << '\n';
            }
            else if (command == "MAX" && arg == "VERTEXES") {
                std::cout << static_cast<int>(maxAreaVer(arg, polygons)) << '\n';
            }
            else if (command == "MIN" && arg == "AREA") {
                std::cout << minAreaVer(arg, polygons) << '\n';
            }
            else if (command == "MIN" && arg == "VERTEXES") {
                std::cout << static_cast<int>(minAreaVer(arg, polygons)) << '\n';
            }
            else {
                invalComm();
            }
        }
        else if (command == "COUNT") {
            std::string arg;
            std::cin >> arg;
            if (arg == "EVEN" || arg == "ODD") {
                std::cout << countVer(arg, polygons) << '\n';
            }
            else {
                try {
                    if (std::stoi(arg) >= 3) {
                        std::cout << countVer(std::stoi(arg), polygons) << '\n';
                    }
                    else {
                        invalComm();
                    }
                }
                catch (...) {
                    invalComm();
                }
            }
        }
        else if (command == "ECHO") {
            Polygon poly;
            std::string line;

            std::getline(std::cin >> std::ws, line);
            std::istringstream iss(line);

            if (!(iss >> poly)) {
                std::cout << "<INVALID COMMAND>\n";
                std::cin.clear();
            }
            else {
                std::cout << echo(polygons, poly) << '\n';
            }
        }
        else if (command == "RIGHTSHAPES") {
            std::cout << rightShapes(polygons) << '\n';
        }
        else {
            invalComm();
        }
    }
}
