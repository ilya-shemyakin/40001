#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <limits>
#include "DataStruct.h"

int main()
{
  using nspace::DataStruct;

  std::vector<DataStruct> data;

  try
  {
    while (!std::cin.eof())
    {
      DataStruct temp;
      if (std::cin >> temp)
      {
        data.push_back(temp);
      }
      else if (!std::cin.eof())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }

    if (data.empty())
    {
      std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
      return 0;
    }

    std::sort(data.begin(), data.end(), nspace::compareDataStruct);

    std::copy(data.begin(), data.end(),
      std::ostream_iterator<DataStruct>(std::cout, "\n"));
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
