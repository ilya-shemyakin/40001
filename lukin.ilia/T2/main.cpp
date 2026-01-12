#include "DataStruct.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int main() {
    std::vector<DataStruct> data;

    std::string line;
    while (std::getline(std::cin, line)) {
        DataStruct ds;
        if (parseDataStruct(line, ds)) {
            data.push_back(ds);
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