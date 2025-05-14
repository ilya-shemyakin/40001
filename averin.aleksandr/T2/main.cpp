#include "DataStruct.hpp"
#include <algorithm>
#include <vector>
#include <iterator>

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



// #include "DataStruct.hpp"
// #include <algorithm>
// #include <vector>
// #include <iterator>
// #include <iostream>
// #include <sstream>
// #include <string>

// int main()
// {
//   std::vector<averin::DataStruct> data;
//   std::string line;

//   while (!std::cin.eof())
//   {
//     if (line.empty()) continue;

//     std::istringstream lineStream(line);
//     averin::DataStruct ds;

//     if (lineStream >> ds)
//     {
//       data.push_back(ds);
//     }
//     else
//     {
//       continue;
//     }
//   }

//   std::sort(data.begin(), data.end(), averin::dataStructCompare);

//   std::copy(
//     data.begin(),
//     data.end(),
//     std::ostream_iterator<averin::DataStruct>(std::cout, "\n")
//   );

//   return 0;
// }
