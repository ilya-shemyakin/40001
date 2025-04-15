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


    bool parseDataStruct(const std::string& input, DataStruct& data);
    bool compareDataStruct(const DataStruct& a, const DataStruct& b);
    std::ostream & operator<<(std::ostream& os, const DataStruct& data);
    std::istream & operator>>(std::istream& is, DataStruct& data);
}

#endif
