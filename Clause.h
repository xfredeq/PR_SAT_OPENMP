//
// Created by frack on 22.05.2022.
//

#ifndef SATSOLVER_CLAUSE_H
#define SATSOLVER_CLAUSE_H

#include <iostream>
#include <string>
#include <vector>
#include<unordered_map>

#include "MyDebugger.h"

class Clause : MyDebugger {
private:
    bool debug;
    std::vector<int> literals;

    static int getLiteralIndex(int literal);

public:
    Clause(): debug(false) {};

    explicit Clause(bool debug, std::vector<int> literals);

    void printClause();

    bool isSatisfiable(bool* currentValues);
};


#endif //SATSOLVER_CLAUSE_H
