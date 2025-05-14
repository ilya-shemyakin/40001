#include "DataStruct.hpp"
#include <algorithm>
#include <vector>
#include <iterator>

int main()
{
  std::vector<averin::DataStruct> data;

  std::copy(
      std::istream_iterator<averin::DataStruct>(std::cin),
      std::istream_iterator<averin::DataStruct>(),
      std::back_inserter(data));

  std::sort(data.begin(), data.end(), averin::dataStructCompare);

  std::copy(
      data.begin(),
      data.end(),
      std::ostream_iterator<averin::DataStruct>(std::cout, "\n"));

  return 0;
}
