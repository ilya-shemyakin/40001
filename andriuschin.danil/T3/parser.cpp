#include "parser.hpp"

#include <iostream>

bool andriuschin::Context::eol()
{
  while (input.peek() != '\n')
  {
    char c = '\0';
    input.get(c);
    if (!std::isspace(c))
    {
      return false;
    }
  }
  return true;
}
