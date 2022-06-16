//
// Created by frack on 22.05.2022.
//

#ifndef SATSOLVER_SATSOLVER_H
#define SATSOLVER_SATSOLVER_H


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <omp.h>

#include "MyDebugger.h"
#include "Clause.h"


class SATSolver : MyDebugger {
private:
    bool debug;
    long variables;
    long clausesQuantity;
    std::vector<Clause> clauses;
    bool* result;
public:
    SATSolver() : debug(false), variables(-1), clausesQuantity(-1), result(nullptr) {};

    explicit SATSolver(bool debug);

    bool loadFromFile(const std::string &fileName);

    int checkClauses(bool* currentValues);

    bool findResult();

    void solve(bool* currentValues, int i, bool* success);

};


#endif //SATSOLVER_SATSOLVER_H
