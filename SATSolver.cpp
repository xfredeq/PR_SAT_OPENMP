//
// Created by frack on 22.05.2022.
//

#include <cstring>
#include "SATSolver.h"

SATSolver::SATSolver(bool debug) : SATSolver() {
    this->debug = debug;
}

bool SATSolver::loadFromFile(const std::string &fileName) {

    std::ifstream inStream;
    inStream.open(fileName);
    if (!inStream) {
        std::cerr << "open file";
        return false;
    }

    std::string line;

    std::getline(inStream, line);
    while (line[0] == 'c' && !inStream.eof()) {
        std::getline(inStream, line);
    }
    if (debug) print(line);


    std::string part;

    for (int i = 0; i < 4; ++i) {
        unsigned long long index = line.find(' ');
        if (index == std::string::npos && i < 3) {
            std::cerr << "Error! Too few parameters on \"problem\" line." << std::endl;
            return false;
        }
        part = line.substr(0, index);
        if (debug) print("part: " + part);

        switch (i) {
            case 0:
                if (part != "p") {
                    std::cerr << "Error! Bad file format, expected \"problem\" line but not found." << std::endl;
                    return false;
                }
                break;
            case 1:
                if (part != "cnf") {
                    std::cerr << "Error! Bad file format, only \"cnf\" format is valid." << std::endl;
                    return false;
                }
                break;
            case 2:
                this->variables = strtol(part.c_str(), nullptr, 10);
                if (part != "0" && this->variables == 0L) {
                    std::cerr << "Error! Bad file format, expected number, got: \"" << part << "\"" << std::endl;
                    return false;
                }
                break;
            case 3:
                this->clausesQuantity = strtol(part.c_str(), nullptr, 10);
                if (part != "0" && this->clausesQuantity == 0L) {
                    std::cerr << "Error! Bad file format, expected number, got: \"" << part << "\"" << std::endl;
                    return false;
                }
                break;
            default:
                break;
        }
        line = line.substr(index + 1);
    }

    std::vector<int> literals;

    int literal;
    while (!inStream.eof()) {
        inStream >> literal;
        while (literal != 0) {

            literal > 0 ? literals.push_back(2 * literal) : literals.push_back(2 * abs(literal) + 1);
            inStream >> literal;
        }
        if (debug) print("size: " + std::to_string(literals.size()));
        this->clauses.emplace_back(Clause(debug, literals));
        literals.clear();
    }


    inStream.close();

    if (debug) print("CLAUSES COLLECTED: " + std::to_string(clauses.size()));
    return true;
}

int SATSolver::checkClauses(bool *currentValues) {
    int quantity = 0;
    for (auto &clause: clauses) {
        if (clause.isSatisfiable(currentValues)) {
            quantity++;
        }
    }
    return quantity;
}

bool SATSolver::findResult() {
    double start = omp_get_wtime();

    this->success = new bool;
    *this->success = false;

    bool *entryValues = new bool[this->variables]();
    solve(entryValues, 0);

    double end = omp_get_wtime();

    print("TIME: " + std::to_string(end - start));

    if (*this->success) {
//        std::cout << "result: " << std::endl;
//        for (int i = 0; i < this->variables; i++) {
//            std::cout << i+1 << "  value: " << result[i] << std::endl;
//        }
        delete[] entryValues;
        return true;
    }
    return false;
}

void SATSolver::solve(bool *currentValues, int i) {
    if (*this->success) return;
    if (i > this->variables) {
        return;
    }
    int satisfied = checkClauses(currentValues);
    if (satisfied == this->clausesQuantity) {
        *this->success = true;
        this->result = currentValues;
        return;
    }


//    bool *c1 = new bool[this->variables]();
//    std::memcpy(c1, currentValues, sizeof(bool) * this->variables);
    solve(currentValues, i + 1);


    bool *c2 = new bool[this->variables]();
    std::memcpy(c2, currentValues, sizeof(bool) * this->variables);
    c2[i] = true;
    solve(c2, i + 1);

//    delete[] c1;
    delete[] c2;
}


