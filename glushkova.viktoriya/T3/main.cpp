#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <limits>
#include <map>
#include <functional>
#include <string>
#include "shapes.hpp"
#include "shape_commands.hpp"

int main(int argc, const char* argv[])
{
    using namespace shapes;

    if (argc != 2)
    {
        std::cerr << "Error: filename parameter required\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Error: cannot open file\n";
        return 1;
    }

    std::vector<Polygon> polygons;

    while (!file.eof())
    {
        Polygon poly;
        if (file >> poly)
        {
            polygons.push_back(poly);
        }
        else
        {
            file.clear();
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::map<std::string, std::function<void(std::istream&, std::ostream&)>> cmds;
    using namespace std::placeholders;

    cmds["AREA"] = std::bind(doArea, std::ref(polygons), _1, _2);
    cmds["MAX"] = std::bind(doMax, std::ref(polygons), _1, _2);
    cmds["MIN"] = std::bind(doMin, std::ref(polygons), _1, _2);
    cmds["COUNT"] = std::bind(doCount, std::ref(polygons), _1, _2);
    cmds["RMECHO"] = std::bind(doRmecho, std::ref(polygons), _1, _2);
    cmds["SAME"] = std::bind(doSame, std::ref(polygons), _1, _2);

    std::string command;
    while (std::cin >> command)
    {
        std::cin >> std::ws;
        auto it = cmds.find(command);
        if (it != cmds.end())
        {
            it->second(std::cin, std::cout);
        }
        else
        {
            std::cout << "<INVALID COMMAND>\n";
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return 0;
}
