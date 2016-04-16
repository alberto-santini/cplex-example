#include "graph.h"
#include "solver.h"
#include <iostream>

int main() {
  using namespace tsp_example;
  
  auto n_nodes = 50u;
  auto graph = Graph(n_nodes);
  auto solver = Solver(graph);
  
  solver.solve_and_print();
  
  return 0;
}