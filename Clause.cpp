//
// Created by frack on 22.05.2022.
//

#include "Clause.h"


Clause::Clause(bool debug, std::vector<int> literals) {
    this->debug = debug;
    this->literals = literals;

    printClause();
}

bool Clause::isSatisfiable(std::unordered_map<int, bool> currentValues) {
    //if (debug) printClause();
    for (auto literal: literals) {
        int index = getLiteralIndex(literal);
        //if (debug) print(("literal: " + std::to_string(literal)));
        //if (debug) print(("index: " + std::to_string(index)));
        if (currentValues.contains(index)) {
            //if (debug) print(("value: " + std::to_string(currentValues[index])));
            if (literal % 2 != currentValues[index]) {
                //print("satisfied");
                return true;
            }
        }
    }
    return false;
}

int Clause::getLiteralIndex(int literal) {
    return literal / 2;
}

void Clause::printClause() {
    for (const auto &literal: literals) {
        int index = getLiteralIndex(literal);
        std::cout << (literal % 2 ? -index : index) << " ";
    }
    std::cout << std::endl;
}
