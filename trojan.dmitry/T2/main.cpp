#include "data_struct.hpp"

#include <iostream>
#include <iomanip>
#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>

bool compare(const trojan::DataStruct& ds1, const trojan::DataStruct& ds2)
{
    if (ds1.key1 != ds2.key1) {
        return ds1.key1 < ds2.key1;
    }
    else if (ds1.key2 != ds2.key2) {
        return ds1.key2 < ds2.key2;
    }
    else {
        return ds1.key3.length() < ds2.key3.length();
    }
}

int main()
{
    std::vector< trojan::DataStruct > data;
    using iter = std::istream_iterator< trojan::DataStruct >;
    while (!std::cin.eof() && !std::cin.bad()) {
        std::copy(iter(std::cin), iter(), std::back_inserter(data));
        if (!std::cin.eof() && !std::cin.bad()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
    }
    std::sort(data.begin(), data.end(), compare);
    std::copy(data.begin(), data.end(), std::ostream_iterator< trojan::DataStruct >(std::cout << std::setprecision(1), "\n"));
}
