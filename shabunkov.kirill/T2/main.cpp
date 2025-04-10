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

  std::string str;

  while (std::getline(std::cin, str))
  {
    if (std::cin.eof()) break;
    std::istringstream iss(str);
    std::copy(
      std::istream_iterator< DataStruct >(iss),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );

    if (iss.fail() && !iss.eof())
    {
      iss.clear();
    }
  }

  std::sort(data.begin(), data.end(), compareDataStruct);

  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );

  return 0;
}

