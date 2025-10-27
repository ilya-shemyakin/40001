#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>
#include "shapes.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
    using vladimirova::Polygon;
    using PolygonIterator = std::istream_iterator<Polygon>;

    if (argc != 2)
    {
        std::cerr << "Error: Incorrect number of arguments\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Cannot open file " << argv[1] << '\n';
        return 1;
    }

    std::vector<Polygon> polygons;
    while (!inputFile.eof())
    {
        std::copy(PolygonIterator{ inputFile }, PolygonIterator{}, std::back_inserter(polygons));
        if (inputFile.fail())
        {
            inputFile.clear();
            inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::map<std::string, std::function<void()>> commands;
    commands["AREA"] = std::bind(vladimirova::processArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
    commands["MAX"] = std::bind(vladimirova::processMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
    commands["MIN"] = std::bind(vladimirova::processMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
    commands["COUNT"] = std::bind(vladimirova::processCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
    commands["ECHO"] = std::bind(vladimirova::processEcho, std::ref(std::cin), std::ref(std::cout), std::ref(polygons));
    commands["MAXSEQ"] = std::bind(vladimirova::processMaxSequence, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

    std::string command;
    while (!(std::cin >> command).eof())
    {
        try
        {
            commands.at(command)();
        }
        catch (const std::exception& e)
        {
            if (std::cin.fail())
            {
                std::cin.clear();
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "<INVALID COMMAND>\n";
        }
    }

    return 0;
}
