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

namespace tsp_example {
  class Solver {
    const Graph& g;
    
    void print_solution(std::ostream& out, const IloCplex& cplex, const IloArray<IloNumVarArray>& x) const;
    
  public:
    explicit Solver(const Graph& g) : g(g) {}
    void solve_and_print() const;
  };
}

#endif