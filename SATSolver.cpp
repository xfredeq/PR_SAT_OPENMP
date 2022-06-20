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

    std::string part;
    for (int i = 0; i < 4; ++i) {
        unsigned long long index = line.find(' ');
        if (index == std::string::npos && i < 3) {
            std::cerr << "Error! Too few parameters on \"problem\" line." << std::endl;
            return false;
        }
        part = line.substr(0, index);

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
        this->clauses.emplace_back(Clause(debug, literals));
        literals.clear();
    }
    inStream.close();
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
    this->success = new bool;
    *this->success = false;
    this->result = new bool[this->variables]();

    this->start = omp_get_wtime();

#pragma omp parallel default(none)
#pragma omp single
    {
        bool *entryValues = new bool[this->variables]();
        int i = 0;
        solve(entryValues, i);
        //delete[] entryValues;
    }
    if (*this->success) {
//        for (int i = 0; i < variables; ++i) {
//            printf(" var %d value %d\n", i + 1, result[i]);
//        }
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
        double end = omp_get_wtime();
        std::cout << (end - start) << std::endl;

        std::memcpy(result, currentValues, sizeof(bool) * this->variables);
//        for (int j = 0; j < variables; ++j) {
//            printf(" var %d value %d\n", j + 1, result[j]);
//        }
        return;
    }

#pragma omp task untied
    {
#pragma omp task
        {
            solve(currentValues, i + 1);
        }


#pragma omp task
        {
            bool *c2 = new bool[this->variables]();
            std::memcpy(c2, currentValues, sizeof(bool) * this->variables);
            c2[i] = true;
            solve(c2, i + 1);
            delete[] c2;
        }
#pragma omp taskwait
    }

#pragma omp taskwait

}


