#include "graph.h"
#include <cmath>
#include <random>
#include <iterator>
#include <algorithm>

namespace tsp_example {
  Graph::Graph(int n_nodes) : n_nodes(n_nodes) {
    generate_random_euclidean_cost_matrix(100);
  }
  
  void Graph::generate_random_euclidean_cost_matrix(int square_size) {
  	std::random_device rd;
  	std::mt19937 mt(rd());
  	std::uniform_int_distribution<int> dist(-square_size, square_size);

  	std::vector<double> x_coord(n_nodes);
  	std::vector<double> y_coord(n_nodes);

  	std::generate(x_coord.begin(), x_coord.end(), [&] () { return dist(mt); });
  	std::generate(y_coord.begin(), y_coord.end(), [&] () { return dist(mt); });

    costs = cost_matrix_type(n_nodes, cost_row_type(n_nodes));

  	for(auto i = 0u; i < costs.size(); ++i) {
      costs[i][i] = square_size * 2;
  		for(auto j = i + 1; j < costs[i].size(); ++j) {
        auto dist = std::sqrt(std::pow(x_coord[i] - x_coord[j], 2.0) + std::pow(y_coord[i] - y_coord[j], 2.0));
				costs[i][j] = dist;
        costs[j][i] = dist;
  		}
  	}
  }
  
  std::ostream& Graph::print(std::ostream& out) const {
    for(const auto& row : costs) {
      std::copy(row.begin(), row.end(), std::ostream_iterator<cost_type>(out, "\t"));
      out << std::endl;
    }
    return out;
  }
}