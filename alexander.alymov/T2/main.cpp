#include "DataStruct.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <limits>

int main() {

    std::vector<alymov::DataStruct> data;

    while (std::cin.good())
    {
        std::copy(
            std::istream_iterator<alymov::DataStruct>(std::cin),
            std::istream_iterator<alymov::DataStruct>(),
            std::back_inserter(data)
        );
        if (std::cin.fail() && !std::cin.eof())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end(), alymov::compareDataStruct);

    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator<alymov::DataStruct>(std::cout, "\n")
    );

    return 0;
}
