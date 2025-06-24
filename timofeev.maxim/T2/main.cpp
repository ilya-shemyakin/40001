#include <iostream>
#include "DataStruct.hpp"

int main()
{
  using timofeev::DataStruct;

  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );
    if (std::cin.fail() && !std::cin.eof() && !std::cin.bad())
    {
      std::cin.clear();
    }
  }
  std::sort(data.begin(), data.end(), timofeev::dataStructCompare);

  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );

  return 0;
}
