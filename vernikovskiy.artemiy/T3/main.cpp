#include <iostream>
#include <fstream>
#include "task.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::invalid_argument("No fn");
        return 1;
    }

    std::string declar;
    std::ifstream file(argv[1]);
    if (!file) {
        throw std::runtime_error("GFYS");
        return 1;
    }

    doomsday::Shapes shapes;
    while (std::getline(file, declar)) {
        shapes.addShape(declar);
    }
    file.close();

    std::string command;
    while (std::getline(std::cin, command))
        shapes.processCommand(command);
    }

    return 0;
}

