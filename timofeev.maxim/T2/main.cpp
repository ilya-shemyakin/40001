#include <iostream>
#include "DataStruct.hpp"


int main()
{
  std::vector < timofeev::DataStruct > data;

  std::string dataObject;

  while (std::getline(std::cin, dataObject))
  {
    std::istringstream iss(dataObject);
    std::copy(
      std::istream_iterator< timofeev::DataStruct >(iss),
      std::istream_iterator< timofeev::DataStruct >(),
      std::back_inserter(data)
    );

    if (iss.fail() && !iss.eof())
    {
      iss.clear();
    }
  }

  std::sort(data.begin(), data.end(), timofeev::dataStructCompare);

  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< timofeev::DataStruct >(std::cout, "\n")
  );

  return 0;
}
