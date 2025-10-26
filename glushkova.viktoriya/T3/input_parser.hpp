#ifndef INPUT_PARSER_HPP
#define INPUT_PARSER_HPP
#include <iostream>

namespace shapes
{
    struct Delimiter
    {
        char expected;
    };
    std::istream& operator>>(std::istream& in, Delimiter&& exp);
}
#endif
