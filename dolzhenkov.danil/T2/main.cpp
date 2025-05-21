#include <iostream>
#include <limits>
#include "DataStruct.h"

int main()
{
    using namespace dolzhenkov;

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
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
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
