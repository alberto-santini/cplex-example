#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>

namespace tsp_example {
  class Graph {
    using cost_type = float;
    using cost_row_type = std::vector<cost_type>;
    using cost_matrix_type = std::vector<cost_row_type>;
    
    uint32_t n_nodes;
    cost_matrix_type costs;
    
    void generate_random(float square_side);
    
  public:
    explicit Graph(uint32_t n_nodes);
    
    uint32_t size() const { return n_nodes; }
    float cost(uint32_t i, uint32_t j) const { return costs[i][j]; }
    friend std::ostream& operator<<(std::ostream& out, const Graph& g);
  };
  
}

#endif