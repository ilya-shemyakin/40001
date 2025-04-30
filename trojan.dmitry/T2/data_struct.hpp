#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iosfwd>
#include <string>

namespace trojan
{
    struct DataStruct
    {
        double key1;
        unsigned long long key2;
        std::string key3;
    };
    std::istream& operator>>(std::istream& stream, DataStruct& value);
    std::ostream& operator<<(std::ostream& stream, const DataStruct& value);

    namespace input
    {
        struct Delimiter
        {
            char expected = '\0';
        };
        std::istream& operator>>(std::istream& stream, Delimiter&& value);
        struct DoubleLiteral
        {
            double& link;
        };
        std::istream& operator>>(std::istream& stream, DoubleLiteral&& value);
        struct UllLiteral
        {
            unsigned long long& link;
        };
        std::istream& operator>>(std::istream& stream, UllLiteral&& value);
        struct StringLiteral
        {
            std::string& link;
        };
        std::istream& operator>>(std::istream& stream, StringLiteral&& value);
    }
    namespace output
    {
        struct DoubleLiteral
        {
            double value;
        };
        std::ostream& operator<<(std::ostream& stream, const DoubleLiteral& value);
        struct UllLiteral
        {
            unsigned long long value;
        };
        std::ostream& operator<<(std::ostream& stream, const UllLiteral& value);
        struct StringLiteral
        {
            std::string value;
        };
        std::ostream& operator<<(std::ostream& stream, const StringLiteral& value);
    }
}

#endif
