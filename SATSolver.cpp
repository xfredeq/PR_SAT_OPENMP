//
// Created by frack on 22.05.2022.
//

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

int SATSolver::checkClauses(const std::unordered_map<int, bool> &currentValues) {
    int quantity = 0;
    for (auto clause: clauses) {
        if (clause.isSatisfiable(currentValues)) {
            quantity++;
            //if (debug) clause.printClause();
        }
    }
    return quantity;
}

bool SATSolver::findResult() {


    bool *flagPointer = new bool;
    *flagPointer = false;

    solve(this->clausesQuantity, {}, 0, flagPointer);

    if (!this->result.empty()) {
        std::cout << "result: " << std::endl;
        for (auto val: result) {
            std::cout << val.first << "  " << val.second << std::endl;
        }
        return true;
    }
    return false;
}

void SATSolver::solve(int n, const std::unordered_map<int, bool> &currentValues, int i, bool *success) {
    if (*success) return;

    //std::cout << "current vals: " << std::endl;
    for (auto val: currentValues) {
        //std::cout << val.first << "  " << val.second << std::endl;
    }
    //std::cout << "end vals" << std::endl;

    int satisfied = checkClauses(currentValues);

    if (satisfied == this->clausesQuantity) {
        *success = true;
        this->result = currentValues;
        print("finished success");
        return;
    } else if (i > 0 && satisfied == 0) {
        //print("finished failed 1");
        return;
    }

    if (i > n) {
        //print("finished failed 2");
        return;
    }
    if (debug) print(std::to_string(i));

    std::unordered_map<int, bool> c1 = currentValues;
    c1.insert({i + 1, false});
    solve(n, c1, i + 1, success);


    std::unordered_map<int, bool> c2 = currentValues;
    c2.insert({i + 1, true});
    solve(n, c2, i + 1, success);


}


