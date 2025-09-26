#include "DataStruct.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <limits>
#include <sstream>

int main()
{
    std::vector<DataStruct> data;
    std::string line;

    while (std::getline(std::cin, line))
    {
        if (line.empty()) continue;

        std::istringstream iss(line);
        DataStruct temp;
        if (iss >> temp)
        {
            data.push_back(temp);
        }
    }

    std::sort(data.begin(), data.end(), compareDataStructs);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
