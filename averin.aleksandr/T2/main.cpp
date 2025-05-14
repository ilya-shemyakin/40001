#include "DataStruct.hpp"
#include <algorithm>
#include <vector>
#include <iterator>
#include <limits>

int main()
{
    std::vector<averin::DataStruct> data;
    while (!std::cin.eof())
    {
      std::copy(
        std::istream_iterator<averin::DataStruct>(std::cin),
        std::istream_iterator<averin::DataStruct>(),
        std::back_inserter(data));
      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
    std::sort(data.begin(), data.end(), averin::dataStructCompare);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<averin::DataStruct>(std::cout, "\n"));

    return 0;
}

