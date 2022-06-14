//
// Created by frack on 22.05.2022.
//

#ifndef SATSOLVER_SATSOLVER_H
#define SATSOLVER_SATSOLVER_H


#include<iostream>
#include<string>
#include<fstream>
#include <sstream>
#include <iterator>
#include <vector>

#include "MyDebugger.h"
#include "Clause.h"


class SATSolver : MyDebugger {
private:
    bool debug;
    long variables;
    long clausesQuantity;
    std::vector<Clause> clauses;
    std::unordered_map<int, bool> result;
public:
    SATSolver() : debug(false), variables(-1), clausesQuantity(-1) {};

    explicit SATSolver(bool debug);

    bool loadFromFile(const std::string &fileName);

    int checkClauses(const std::unordered_map<int, bool> &currentValues);

    bool findResult();

    void solve(int n, const std::unordered_map<int, bool>& currentValues, int i, bool* success);

};


#endif //SATSOLVER_SATSOLVER_H