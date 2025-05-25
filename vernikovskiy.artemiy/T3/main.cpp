#include <iostream>
#include <fstream>
#include <sstream>
#include "task.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "ERROR: No fn" << std::endl;
        return 2;
    }

    std::string declar;
    std::ifstream file(argv[1]);
    if (!file) {
        std::cout << "ERROR: GFYS" << std::endl;
        return 2;
    }

    doomsday::Shapes shapes;
    shapes.addShape(file);
    file.close();


    workable::Wrapper wrap(std::cin, std::cout);
    shapes.processCommand(wrap);

    return 0;
}

