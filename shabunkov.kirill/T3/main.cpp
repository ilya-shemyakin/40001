#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>

#include "commands.h"
#include "Polygon.h"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "no filename\n";
    return -1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cerr << "ERROR: cannot open file\n";
    return 1;
  }

  std::vector<Polygon> polygons;
  Polygon poly;

  while (input >> poly || !input.eof())
  {
    if (input.fail())
    {
      input.clear();
      input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }
    polygons.push_back(poly);
  }

  processCommands(polygons);

  return 0;
}

