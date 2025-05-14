#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <functional>
#include <iomanip>
#include <map>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iterator>
#include <limits>
#include "code.hpp"


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "no filename\n";
    return -1;
  }
  using timofeev::Polygon;

  std::vector< Polygon > data;
  std::ifstream file(argv[1]);
  while (!file.eof())
  {
    std::copy(
      std::istream_iterator< Polygon >(file),
      std::istream_iterator< Polygon >(),
      std::back_inserter(data)
    );
    if (file.fail() && !file.eof()) {
      file.clear();
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::map<std::string, std::function<void(const std::vector< Polygon >&, std::string&)>> commands =
  {
    {"AREA", timofeev::AREA},
    {"MAX", timofeev::MAX},
    {"MIN", timofeev::MIN},
    {"COUNT", timofeev::COUNT},
    {"MAXSEQ", timofeev::MAXSEQ},
    {"RECTS", timofeev::RECTS},
  };

  std::string key;
  while (std::cin >> key)
  {
    try
    {
      std::string arg;
      std::getline(std::cin, arg);
      arg.erase(0, 1);

      auto it = commands.find(key);
      if (it != commands.end()) {
        it->second(data, arg);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
