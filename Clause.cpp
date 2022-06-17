//
// Created by frack on 22.05.2022.
//

#include "Clause.h"


Clause::Clause(bool debug, std::vector<int> literals) {
    this->debug = debug;
    this->literals = literals;
}

bool Clause::isSatisfiable(const bool *currentValues) {
    bool result = false;
    int index, i, literal;

#pragma omp parallel private(i, index) shared(result, currentValues)
    {
#pragma omp for
        for (i = 0; i < literals.size(); i++) {
            literal = literals[i];
            index = getLiteralIndex(literal);
            if ((literal & 1) != currentValues[index - 1]) {
                result = true;
            }
        }
    }
    return result;
}

int Clause::getLiteralIndex(int literal) {
    return (literal >> 1);
}

void Clause::printClause() {
    for (const auto &literal: literals) {
        int index = getLiteralIndex(literal);
        std::cout << (literal % 2 ? -index : index) << " ";
    }
    std::cout << std::endl;
}
