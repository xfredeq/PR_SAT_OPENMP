cmake_minimum_required(VERSION 3.22)
project(SATSolver)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

add_executable(SATSolver main.cpp SATSolver.cpp SATSolver.h Clause.cpp Clause.h MyDebugger.cpp MyDebugger.h)
target_link_libraries(SATSolver PRIVATE OpenMP::OpenMP_CXX)