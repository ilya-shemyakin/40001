#include <iostream>
#include <iomanip>
#include <string>
#include <map>

#include "parser.hpp"
#include "polygon.hpp"
#include "processor.hpp"

int main(int argc, char** argv)
{
  std::vector< andriuschin::Polygon > polygons;
  andriuschin::MainProcessor processor;
  andriuschin::Parser< andriuschin::MainProcessor >::map_type comands = {
    {"AREA", &andriuschin::MainProcessor::area},
    {"MAX", &andriuschin::MainProcessor::max},
    {"MIN", &andriuschin::MainProcessor::min},
    {"COUNT", &andriuschin::MainProcessor::count},
    {"LESSAREA", &andriuschin::MainProcessor::lessArea},
    {"INTERSECTIONS", &andriuschin::MainProcessor::intersections}
  };
  std::cout << std::fixed << std::setprecision(1);
  andriuschin::Parser< andriuschin::MainProcessor > parser({}, {std::cin, std::cout, std::cerr, polygons},
      std::move(comands));
  if (!processor.init(parser.context, argc, argv))
  {
    return 1;
  }
  while (parser.run())
  {}
}
