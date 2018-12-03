#ifndef SOLVER_H
#define SOLVER_H

#include "graph.h"
#include <iostream>

// Magic tricks to have CPLEX behave well:
#ifndef IL_STD
#define IL_STD
#endif
#include <cstring>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
// End magic tricks

namespace cplex_example {
  class Solver {
    // The graph on which we are solving the TSP.
    const Graph& g;
    
    // Prints the integer solution obtained by CPLEX to stdout.
    void print_solution(const IloCplex& cplex, const IloArray<IloNumVarArray>& x) const;
    
  public:
    
    // Builds a solver for graph g.
    explicit Solver(const Graph& g) : g{g} {}
    
    // Solves the TSP with CPLEX and prints the result.
    void solve_and_print() const;
  };
}

#endif