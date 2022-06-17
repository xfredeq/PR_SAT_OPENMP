//
// Created by frack on 22.05.2022.
//

#include "Clause.h"


Clause::Clause(bool debug, std::vector<int> literals) {
    this->debug = debug;
    this->literals = literals;
}

bool Clause::isSatisfiable(std::unordered_map<int, bool> currentValues) {
    for (auto literal: literals) {
        int index = getLiteralIndex(literal);
        if (currentValues.contains(index)) {
            if ((literal & 1) != currentValues[index]) {
                return true;
            }
        }
    }
    return false;
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
