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

    std::sort(data.begin(), data.end(), [](const DataStruct& a, const DataStruct& b) { //компаратор
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
        });


    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
