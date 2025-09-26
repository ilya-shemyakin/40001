#include <limits>
#include <fstream>
#include "commands.h"

int main(int nArguments, char** file)
{
    using namespace alymov;
    if (nArguments < 2)
    {
        return 1;
    }

    std::ifstream inputF(file[1]);
    if (!inputF)
    {
        return 1;
    }

    std::vector< Polygon > polygon;
    while (!inputF.eof()) {
        std::copy(
            std::istream_iterator<Polygon>(inputF),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygon)
        );
        if (inputF.fail() && !inputF.eof()) {
            inputF.clear();
        }
    }
    std::cin.clear();

    std::string line;
    while (std::getline(std::cin, line))
    {
        std::istringstream iss(line);
        std::string command;
        while (iss >> command)
        {
            if (command == "AREA")
            {
                std::string mode;
                if (iss >> mode)
                {
                    area(polygon, mode, std::cout);
                }
                else
                {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                }
            }
            else if (command == "MAX")
            {
                std::string mode;
                if (iss >> mode)
                {
                    max(polygon, mode, std::cout);
                }
                else
                {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                }
            }
            else if (command == "MIN")
            {
                std::string mode;
                if (iss >> mode)
                {
                    min(polygon, mode, std::cout);
                }
                else
                {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                }
            }
            else if (command == "COUNT")
            {
                std::string mode;
                if (iss >> mode)
                {
                    count(polygon, mode, std::cout);
                }
                else
                {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                }
            }
            else if (command == "LESSAREA")
            {
                lessarea(polygon, iss, std::cout);
                if (iss.fail())
                {
                    iss.clear();
                    iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            else if (command == "MAXSEQ")
            {
                maxseq(polygon, iss, std::cout);
                if (iss.fail())
                {
                    iss.clear();
                    iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            else
            {
                std::cout << "<INVALID COMMAND>" << std::endl;
                iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    return 0;
}
