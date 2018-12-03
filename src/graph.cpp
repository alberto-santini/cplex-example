#include "graph.h"
#include <cmath>
#include <random>
#include <iterator>
#include <algorithm>

namespace cplex_example {
  Graph::Graph(std::uint32_t n_nodes) : n_nodes{n_nodes} {
    generate_random(100);
  }
  
  void Graph::generate_random(float square_side) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0, square_side);

    std::vector<float> x_coord(n_nodes);
    std::vector<float> y_coord(n_nodes);

    std::generate(x_coord.begin(), x_coord.end(), [&] () { return dist(mt); });
    std::generate(y_coord.begin(), y_coord.end(), [&] () { return dist(mt); });

    costs = std::vector<std::vector<float>>(n_nodes, std::vector<float>(n_nodes));

    for(auto i = 0u; i < costs.size(); ++i) {
      costs[i][i] = 999 * square_side;
      for(auto j = i + 1; j < costs[i].size(); ++j) {
        const auto dist = std::sqrt(std::pow(x_coord[i] - x_coord[j], 2.0) + std::pow(y_coord[i] - y_coord[j], 2.0));
        costs[i][j] = dist;
        costs[j][i] = dist;
      }
    }
  }
  
  std::ostream& operator<<(std::ostream& out, const Graph& g) {
    for(const auto& row : g.costs) {
      std::copy(row.begin(), row.end(), std::ostream_iterator<float>(out, "\t"));
      out << "\n";
    }
    return out;
  }
}