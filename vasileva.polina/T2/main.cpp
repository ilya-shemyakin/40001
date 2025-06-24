#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include "DataStruct.h"


int main()
{
    using vasileva::DataStruct;
    using vasileva::compareDataStruct;

    std::vector<DataStruct> data;

    std::string line;


    while (!std::cin.eof() && !std::cin.bad()) {
        using iter = std::istream_iterator<vasileva::DataStruct>;
        std::copy(iter(std::cin), iter(), std::back_inserter(data));
        if (std::cin.fail() && !std::cin.eof() && !std::cin.bad()) {
            std::cin.clear();
            std::cin.ignore();
        }
    }

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
