#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <map>
#include <functional>
#include <limits>
#include "shapesCommandsSet.hpp"


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

  std::map<std::string, std::function<void(const std::vector< Polygon >&, std::istream& in, std::ostream& out)>> commands =
  {
    {"AREA", timofeev::area},
    {"MAX", timofeev::max},
    {"MIN", timofeev::min},
    {"COUNT", timofeev::count},
    {"MAXSEQ", timofeev::maxseq},
    {"RECTS", timofeev::rects},
  };

  std::string key;
  while (std::cin >> key)
  {
    try
    {
      std::string argLine;
      std::getline(std::cin, argLine);
      std::istringstream argStream(argLine);

      auto it = commands.find(key);
      if (it != commands.end())
      {
        it->second(data, argStream, std::cout);
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
