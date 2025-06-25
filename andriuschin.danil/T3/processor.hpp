#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <algorithm>
#include <iostream>
#include <functional>
#include "parser.hpp"
#include "polygon.hpp"

namespace andriuschin
{
  struct MainProcessor
  {
    bool init(Context& context, int argc, char** argv);
    bool area(Context& context);
    bool max(Context& context);
    bool min(Context& context);
    bool count(Context& context);
    bool lessArea(Context& context);
    bool intersections(Context& context);
  };

  struct AreaProccessor
  {
    bool vertexes(Context& context, size_t num);
    bool odd(Context& context);
    bool even(Context& context);
    bool mean(Context& context);
  };

  template< bool isMax >
  struct MinMaxProcessor
  {
    using double_compare = std::conditional_t< isMax, std::less<>, std::greater<> >;
    bool area(Context& context);
    bool vertexes(Context& context);
  };

  struct CountProcessor
  {
    bool vertexes(Context& context, size_t num);
    bool odd(Context& context);
    bool even(Context& context);
  };
}

template< bool isMax >
bool andriuschin::MinMaxProcessor< isMax >::area(Context& context)
{
  if (context.polygons.empty() || !context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  static auto areaCompare = std::bind(double_compare{}, std::bind(GetArea{}, _1), std::bind(GetArea{}, _2));
  double area = GetArea{}(*std::max_element(context.polygons.begin(), context.polygons.end(), areaCompare));

  context.output << area << '\n';
  return true;
}

template< bool isMax >
bool andriuschin::MinMaxProcessor< isMax >::vertexes(Context& context)
{
  if (context.polygons.empty() || !context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  static auto getData = std::bind(std::mem_fn(&Polygon::points), _1);
  static auto getDataSize = std::bind(&decltype(Polygon::points)::size, getData);
  static auto vertexCompare = std::bind(double_compare{}, std::bind(getDataSize, _1), std::bind(getDataSize, _2));
  size_t count = std::max_element(context.polygons.begin(), context.polygons.end(), vertexCompare)->points.size();

  context.output << count << '\n';
  return true;
}

#endif
