#include <iostream>
#include <vector>
#include <iterator>
#include <limits>
#include <algorithm>

#include "data_struct.hpp"

int main()
{
  std::vector< andriuschin::DataStruct > vector;
  while (!std::cin.eof() && !std::cin.bad())
  {
    using IstremIter = std::istream_iterator< andriuschin::DataStruct >;
    std::copy(IstremIter(std::cin), IstremIter(), std::back_inserter(vector));
    if (std::cin.fail() && !std::cin.eof() && !std::cin.bad())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(vector.begin(), vector.end(), andriuschin::DsCompare());
  using OstreamIter = std::ostream_iterator< andriuschin::DataStruct >;
  std::copy(vector.begin(), vector.end(), OstreamIter(std::cout, "\n"));
}
