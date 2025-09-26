#include <iostream>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

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
      std::cin.ignore();
    }
  }

  std::sort(data.begin(), data.end(), compareDataStruct);

  using OstreamIter = std::ostream_iterator< nspace::DataStruct >;
  std::copy(data.begin(), data.end(), OstreamIter(std::cout, "\n"));
  return 0;
}

