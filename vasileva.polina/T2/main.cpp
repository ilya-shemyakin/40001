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
    using nspace::DataStruct;
    using nspace::compareDataStruct;

    std::vector<DataStruct> data;

    std::string line;


    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::copy(
            std::istream_iterator<DataStruct>(iss),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );


        if (iss.fail() && !iss.eof()) {
            iss.clear();
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
