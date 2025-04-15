#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "task.h"
#include "tests.h"


int main() {
    using namespace doomsday;
    std::vector<DataStruct> dataVec;

    DataStruct data;
    while (!std::cin.eof()) {
        if (std::cin >> data)
            dataVec.push_back(data);
//        std::copy(
//            std::istream_iterator<DataStruct>(std::cin),
//            std::istream_iterator<DataStruct>(),
//            std::back_inserter(dataVec)
//        );

        if (std::cin.fail() && !std::cin.eof())
        {
          std::cin.clear();
        }
    }


    std::sort(dataVec.begin(), dataVec.end(), compareDataStruct);

    std::copy(dataVec.begin(), dataVec.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
