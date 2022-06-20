#include <iostream>

#include "SATSolver.h"

bool debug = false;

int main(int argc, char *argv[]) {
    omp_set_dynamic(0);
    std::string fileName = "inputfile.txt";
    int threads = 4;

    if (argc > 2) {
        try {
            fileName = argv[1];
            threads = std::stoi(argv[2]);
        } catch (const std::exception &e) {
            printf("First argument - file name, second - number of threads.");
        }
    } else if (argc > 1) {
        try {
            fileName = argv[1];
        } catch (const std::exception &e) {
            printf("First argument - file name, second - number of threads.");
        }
    }

    omp_set_num_threads(threads);

    SATSolver solver(debug);
    if (!solver.loadFromFile(fileName)) {
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
