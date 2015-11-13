#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>

namespace tsp_example {
  class Graph {
    using cost_type = double;
    using cost_row_type = std::vector<cost_type>;
    using cost_matrix_type = std::vector<cost_row_type>;
    
    int n_nodes;
    cost_matrix_type costs;
    
    void generate_random_euclidean_cost_matrix(int square_size);
    
  public:
    explicit Graph(int n_nodes);
    std::ostream& print(std::ostream& out) const;
    
    int size() const { return n_nodes; }
    double cost(int i, int j) const { return costs[i][j]; }
  };
}

#endif