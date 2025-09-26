#include <iostream>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

#include "DataStruct.h"

int main()
{
  using nspace::DataStruct;
  using nspace::compareDataStruct;

  std::vector< DataStruct >  data;

  while (!std::cin.eof() && !std::cin.bad())
  {
    using IstremIter = std::istream_iterator< nspace::DataStruct >;
    std::copy(IstremIter(std::cin), IstremIter(), std::back_inserter(data));
    if (std::cin.fail() && !std::cin.eof() && !std::cin.bad())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), compareDataStruct);

  using OstreamIter = std::ostream_iterator< nspace::DataStruct >;
  std::copy(data.begin(), data.end(), OstreamIter(std::cout, "\n"));
  std::cout << std::endl;
  return 0;
}

