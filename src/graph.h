#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>

namespace cplex_example {
  class Graph {
    // Number of nodes in the graph.
    // Nodes go from 0 to n_nodes - 1.
    const std::uint32_t n_nodes;
    
    // Arc costs matrix. For every pair of
    // nodes i,j (with 0 < i,j < n_nodes)
    // it gives the cost of arc (i,j).
    std::vector<std::vector<float>> costs;
    
    // Generates a random graph with euclidean
    // distances and nodes randomly placed inside
    // a square of side square_side.
    void generate_random(float square_side);
    
  public:
    
    // Created a new (random) graph with n_nodes nodes
    explicit Graph(std::uint32_t n_nodes);
    
    // Size of the graph (i.e. number of nodes).
    std::uint32_t size() const { return n_nodes; }
    
    // Cost of arc (i,j).
    float cost(std::uint32_t i, std::uint32_t j) const { return costs[i][j]; }
    
    // Prints out the cost matrix of the graph.
    friend std::ostream& operator<<(std::ostream& out, const Graph& g);
  };
}

#endif