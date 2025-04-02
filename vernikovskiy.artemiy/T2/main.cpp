#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "task.h"
#include "tests.h"


int main() {
    std::vector<DataStruct> dataVec;
    std::string line;

    DataStruct data;
    while (!std::cin.eof()) {
        std::cin.clear();
        if (std::cin >> data)
            dataVec.push_back(data);

    }


    std::sort(dataVec.begin(), dataVec.end(), compareDataStruct);

    std::copy(dataVec.begin(), dataVec.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
