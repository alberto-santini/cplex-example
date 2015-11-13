#include "solver.h"
#include <cmath>
#include <limits>

namespace tsp_example {
  void Solver::solve_and_print() const {
  	auto n = g.size();

  	// CPLEX environment. Takes care of everything, including memory management for CPLEX objects.
  	IloEnv env;

  	// CPLEX model. We put variables and constraints in it!
  	IloModel model(env);

  	// Model:
  	//
  	// BINARY VARIABLE x[i][j]		For all i,j = 0, ..., n - 1
  	//		x[i][j] == 1			      If arc (i,j) is selected
  	//		x[i][j] == 0			      Otherwise
  	//
  	// INTEGER VARIABLE t[i]		For all i = 0, ..., n - 1
  	//		t[i] == k					    Iff node i is the k-th node in the tour
  	//		t[0] == 1
  	//		t[i] in [2, ..., n]	  For all i = 1, ... n - 1
  	//
  	// OBJECTIVE FUNCTION
  	//		MIN sum((i,j), c[i][j] * x[i][j])
  	//
  	// CONSTRAINTS
  	//	1) sum(j, x[j][i]) == 1							          For all i
  	//  2) sum(j, x[i][j]) == 1							          For all i
  	//	3) t[i] - t[j] + 1 <= n * (1 - x[i][j])     	For all i,j = 1, ..., n - 1
  	//		Can be written as:
  	//		t[i] - t[j] + n * x[i][j] <= n - 1

  	// Variables
  	IloArray<IloNumVarArray> x(env, n);
  	IloNumVarArray t(env, n);

  	// Constraints
  	IloRangeArray inbound_arcs(env, n);
  	IloRangeArray outbound_arcs(env, n);
  	IloArray<IloRangeArray> mtz(env, n);

  	std::stringstream name;

  	// Create variable t[0] and fix it to value 1
    // This breaks symmetry, because it fixes node 0 as the starting node of the tour
  	t[0] = IloNumVar(env, 1, 1, IloNumVar::Int, "t_0");

  	// Create variables t[1], ..., t[n]
  	for(auto i = 0; i < n; ++i) {
  		name << "t_" << i;
  		t[i] = IloNumVar(env, 2, n, IloNumVar::Int, name.str().c_str());
  		name.str(""); // Clean name
  	}

  	// Create variables x
  	for(auto i = 0; i < n; ++i) {
  		x[i] = IloNumVarArray(env, n);
  		for(auto j = 0; j < n; ++j) {
  			name << "x_" << i << "_" << j;
  			x[i][j] = IloNumVar(env, 0, 1, IloNumVar::Bool, name.str().c_str());
  			name.str(""); // Clean name
  		}
  	}

  	IloExpr expr(env);

  	// Create constraints 1)
  	for(auto i = 0; i < n; ++i) {
  		for(auto j = 0; j < n; ++j) {
  			expr += x[j][i];
  		}

  		name << "inbound_" << i;
  		inbound_arcs[i] = IloRange(env, 1, expr, 1, name.str().c_str());
  		name.str(""); // Clean name
  		expr.clear(); // Clean expr
  	}

  	// Add constraints to the model
  	model.add(inbound_arcs);

  	// Create constraints 2)
  	for(auto i = 0; i < n; ++i) {
  		for(auto j = 0; j < n; ++j) {
  			expr += x[i][j];
  		}

  		name << "outbound_" << i;
  		outbound_arcs[i] = IloRange(env, 1, expr, 1, name.str().c_str());
  		name.str(""); // Clean name
  		expr.clear(); // Clean expr
  	}

  	// Add constraints 2) to the model
  	model.add(outbound_arcs);

  	// Create constraints 3)
  	// The constraint is for i = 1,...,n and therefore we add empty constraints for i == 0
  	mtz[0] = IloRangeArray(env);
  	// The we continue normally for all other i > 0
  	for(auto i = 1; i < n; ++i) {
  		mtz[i] = IloRangeArray(env, n);
  		for(auto j = 1; j < n; ++j) {
  			expr = t[i] - t[j] + n * x[i][j];

  			name << "mtz_" << i << "_" << j;
  			mtz[i][j] = IloRange(env, -IloInfinity, expr, n - 1, name.str().c_str());
  			name.str(""); // Clean name
  			expr.clear(); // Clean expr
  		}
  		// Add constraints 3)[i] to the model
  		model.add(mtz[i]);
  	}

  	// Create objective function
  	for(auto i = 0; i < n; ++i) {
  		for(auto j = 0; j < n; ++j) {
  			expr += g.cost(i, j) * x[i][j];
  		}
  	}
  	IloObjective obj(env, expr, IloObjective::Minimize);

  	// Add the objective function to the model
  	model.add(obj);

  	// Create the solver object
  	IloCplex cplex(model);

  	// Export model to file (useful to debug!)
  	cplex.exportModel("model.lp");
    
    bool solved = false;
    
    try {
      solved = cplex.solve();
    } catch(const IloException& e) {
      std::cerr << std::endl << std::endl;
      std::cerr << "CPLEX Raised an exception:" << std::endl;
      std::cerr << e << std::endl;
      env.end();
      throw;
    }

  	if(solved) {
  		std::cout << std::endl << std::endl;
  		std::cout << "Cplex success!" << std::endl;
  		std::cout << "\tStatus: " << cplex.getStatus() << std::endl;
  		std::cout << "\tObjective value: " << cplex.getObjValue() << std::endl;
  		print_solution(std::cout, cplex, x);
  	} else {
  		std::cerr << "Cplex error!" << std::endl;
  		std::cerr << "\tStatus: " << cplex.getStatus() << std::endl;
  		std::cerr << "\tSolver status: " << cplex.getCplexStatus() << std::endl;
  	}

  	env.end();
  }
  
  void Solver::print_solution(std::ostream& out, const IloCplex& cplex, const IloArray<IloNumVarArray>& x) const {
  	auto n = g.size();
    auto almost_equal = [] (double x, double y) {
      double magnitude = 10e3;
      return std::abs(x-y) < magnitude * std::numeric_limits<double>::epsilon() * std::abs(x+y) || std::abs(x-y) < std::numeric_limits<double>::min();
    };

    assert(x.getSize() == n);

  	out << std::endl << std::endl << "Solution:" << std::endl;
  	for(auto i = 0; i < n; ++i) {
      assert(x[i].getSize() == n);
  		for(auto j = 0; j < n; ++j) {
  			if(almost_equal(cplex.getValue(x[i][j]), 1)) {
  				out << i << " -> " << j << std::endl;
  			}
  		}
  	}
  }
}