#include "processor.hpp"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <functional>
#include <numeric>
#include <limits>
#include "parser.hpp"

namespace andriuschin
{
  using namespace std::placeholders;
  static auto getData = std::bind(std::mem_fn(&Polygon::points), _1);
  static auto getDataSize = std::bind(&decltype(Polygon::points)::size, getData);
  static auto isEven = std::bind(std::equal_to<>{}, std::bind(std::modulus<>{}, getDataSize, 2), 0);
  static auto isOdd = std::bind(std::logical_not<>{}, isEven);
  static auto getArea = std::bind(GetArea{}, _1);
  static std::function< double(double, const Polygon&) > adder = std::bind(std::plus<>{}, _1, std::bind(GetArea{}, _2));
  using polygons_iter = Context::polygons_container::iterator;
  static auto accumulate = static_cast< double(*)(polygons_iter, polygons_iter,
          double, decltype(adder)) >(std::accumulate);
  static auto getSum = std::bind(accumulate, _1, _2, 0.0, adder);
}

bool andriuschin::MainProcessor::init(Context& context, int argc, char** argv)
{
  if (argc != 2)
  {
    context.error << "Wrong arguments vertexes (expected 2)\n";
    return false;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    context.error << "Failed to open file\n";
    return false;
  }
  using iterator = std::istream_iterator< Polygon >;
  while (file)
  {
    std::copy(iterator(file), iterator(), std::back_inserter(context.polygons));
    file.clear(file.rdstate() & ~std::ios::failbit);
    file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return true;
}

bool andriuschin::MainProcessor::area(Context& context)
{
  static Parser< AreaProccessor >::map_type comands = {
    {"EVEN", &AreaProccessor::even},
    {"ODD", &AreaProccessor::odd},
    {"MEAN", &AreaProccessor::mean}
  };
  static Parser< AreaProccessor > parser({}, context, std::move(comands));
  parser.regiserNumCall(&AreaProccessor::vertexes);
  return parser.run();
}
bool andriuschin::AreaProccessor::vertexes(Context& context, size_t num)
{
  if ((num < 3) || !context.eol())
  {
    return false;
  }
  auto isExpectedSize = std::bind(std::equal_to<>{}, getDataSize, num);
  auto workspaceEnd = std::partition(context.polygons.begin(), context.polygons.end(), isExpectedSize);
  context.output << getSum(context.polygons.begin(), workspaceEnd) << '\n';
  return true;
}
bool andriuschin::AreaProccessor::even(Context& context)
{
  if (!context.eol())
  {
    return false;
  }
  auto workspaceEnd = std::partition(context.polygons.begin(), context.polygons.end(), isEven);
  context.output << getSum(context.polygons.begin(), workspaceEnd) << '\n';
  return true;
}
bool andriuschin::AreaProccessor::odd(Context& context)
{
  if (!context.eol())
  {
    return false;
  }
  auto workspaceEnd = std::partition(context.polygons.begin(), context.polygons.end(), isOdd);
  context.output << getSum(context.polygons.begin(), workspaceEnd) << '\n';
  return true;
}
bool andriuschin::AreaProccessor::mean(Context& context)
{
  if (context.polygons.empty() || !context.eol())
  {
    return false;
  }
  context.output << getSum(context.polygons.begin(), context.polygons.end()) / context.polygons.size() << '\n';
  return true;
}

bool andriuschin::MainProcessor::min(Context& context)
{
  static Parser< MinMaxProcessor< false > >::map_type comands = {
    {"AREA", &MinMaxProcessor< false >::area},
    {"VERTEXES", &MinMaxProcessor< false >::vertexes}
  };
  static Parser< MinMaxProcessor< false > > parser({}, context, std::move(comands));
  return parser.run();
}
bool andriuschin::MainProcessor::max(Context& context)
{
  static Parser< MinMaxProcessor< true > >::map_type comands = {
    {"AREA", &MinMaxProcessor< true >::area},
    {"VERTEXES", &MinMaxProcessor< true >::vertexes}
  };
  static Parser< MinMaxProcessor< true > > parser({}, context, std::move(comands));
  return parser.run();
}

bool andriuschin::MainProcessor::count(Context& context)
{
  context.input.clear(context.input.rdstate() & ~std::ios::failbit);
  static Parser< CountProcessor >::map_type comands = {
    {"EVEN", &CountProcessor::even},
    {"ODD", &CountProcessor::odd}
  };
  static Parser< CountProcessor > parser({}, context, std::move(comands));
  parser.regiserNumCall(&CountProcessor::vertexes);
  return parser.run();
}
bool andriuschin::CountProcessor::vertexes(Context& context, size_t num)
{
  if ((num < 3) || !context.eol())
  {
    return false;
  }
  auto isExpectedSize = std::bind(std::equal_to<>{}, getDataSize, num);
  context.output << std::count_if(context.polygons.begin(), context.polygons.end(), isExpectedSize) << '\n';
  return true;
}
bool andriuschin::CountProcessor::even(Context& context)
{
  if (!context.eol())
  {
    return false;
  }
  context.output << std::count_if(context.polygons.begin(), context.polygons.end(), isEven) << '\n';
  return true;
}
bool andriuschin::CountProcessor::odd(Context& context)
{
  if (!context.eol())
  {
    return false;
  }
  context.output << std::count_if(context.polygons.begin(), context.polygons.end(), isOdd) << '\n';
  return true;
}
bool andriuschin::MainProcessor::lessArea(Context& context)
{
  Polygon polygon;
  if (!(context.input >> polygon) || !context.eol())
  {
    return false;
  }
  auto compare = std::bind(std::less<>{}, getArea, GetArea{}(polygon));
  context.output << std::count_if(context.polygons.begin(), context.polygons.end(), compare) << '\n';
  return true;
}
bool andriuschin::MainProcessor::intersections(Context& context)
{
  Polygon polygon;
  if (!(context.input >> polygon) || !context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  auto intersect = std::bind(GetIntersections{}, std::cref(polygon), _1);
  context.output << std::count_if(context.polygons.begin(), context.polygons.end(), intersect) << '\n';
  return true;
}
