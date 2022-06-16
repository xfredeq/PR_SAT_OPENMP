#include <iostream>

#include "SATSolver.h"

bool debug = false;

int main() {
    SATSolver solver(debug);
    if (!solver.loadFromFile("inputfile.txt")) {
        return 1;
    }

    if (solver.findResult()) {
        std::cout << "found" << std::endl;
    } else {
        std::cout << "none" << std::endl;
    }
    return 0;
}
