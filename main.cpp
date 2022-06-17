#include <iostream>

#include "SATSolver.h"

bool debug = false;

int main(int argc, char *argv[]) {

    if (argc > 1) {
        int threads = std::stoi(argv[1]);
        omp_set_num_threads(threads);
    } else {
        omp_set_num_threads(1);
    }
    SATSolver solver(debug);
    if (!solver.loadFromFile("inputfile.txt")) {
        return 1;
    }
    solver.findResult();
//    if (solver.findResult()) {
//        std::cout << "found" << std::endl;
//    } else {
//        std::cout << "not found" << std::endl;
//    }
    return 0;
}
