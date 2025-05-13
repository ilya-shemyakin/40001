#include <iostream>
#include <fstream>
#include <sstream>
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

    shapes.print();
//    std::string command;
//    while (std::getline(std::cin, command)) {
//        std::stringstream scommand(command);
//        shapes.processCommand(scommand);
//    }
    workable::Wrapper wrap(std::cin, std::cout);
    shapes.processCommand(wrap);
    
    return 0;
}

