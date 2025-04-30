#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <string>

namespace doomsday
{

    struct DataStruct {
        double key1;
        char key2;
        std::string key3;
    };
// okay, it makes sense, but without debugger it is very painful
    struct DelimiterIO
    {
        char delim;
    };
    struct keyLable
    {
        std::string key;
    };
    struct DoubleIO
    {
        double& ref;
    };
    struct CharIO
    {
        char& ref;
    };
    struct StringIO
    {
        std::string& ref;
    };

    std::istream& operator>>(std::istream& is, keyLable& dest);
    std::istream& operator>>(std::istream& is, DelimiterIO&& delim);
    std::istream& operator>>(std::istream& is, DoubleIO&& dest);
    std::istream& operator>>(std::istream& is, CharIO&& dest);
    std::istream& operator>>(std::istream& is, StringIO&& dest);


    bool compareDataStruct(const DataStruct& a, const DataStruct& b);
    std::ostream & operator<<(std::ostream& os, const DataStruct& data);
    std::istream& operator>>(std::istream& is, DataStruct& data);
    std::string formatScientific(double number);
}

#endif
