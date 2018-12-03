#include "graph.h"
#include "solver.h"
#include <iostream>

int main() {
  using namespace cplex_example;
  
  const auto n_nodes = 40u;
  const auto graph = Graph(n_nodes);
  const auto solver = Solver(graph);
  
  solver.solve_and_print();
  
  return 0;
}