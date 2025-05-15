#include <iostream>
#include "DataStruct.h"

int main()
{
    using namespace nspace;

    std::vector< DataStruct > data;

    while (!std::cin.eof())
    {
        std::copy(
            std::istream_iterator< DataStruct >(std::cin),
            std::istream_iterator< DataStruct >(),
            std::back_inserter(data)
        );
        if (std::cin.fail() && !std::cin.eof() && !std::cin.bad())
        {
            std::cin.clear();
        }
    }
    std::sort(data.begin(), data.end(), dataStructCompare);
    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator< DataStruct >(std::cout, "\n")
    );

    return 0;
}

