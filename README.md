## Small CPLEX Callable API tutorial

### Prerequisites

* CPLEX must be installed. I assume that the installation directory is `/opt/ibm`. If you installed CPLEX somewhere else, edit `CPLEX_ROOT_DIR` in `CMakeLists.txt`.
* A modern version of GCC that supports C++14 is required.

### Compile and run

* Create a build directory within the root directory of this project: `mkdir build`.
* Move to the build directory: `cd build`.
* Run cmake: `CXX=/path/to/your/g++ cmake -DCMAKE_BUILD_TYPE=Debug ..`.
  * Substitute the path of your g++ appropriately, or leave out the part with `CXX` if g++ is in your `PATH`.
  * Change `Debug` into `Release` if you want to compile in release mode.
* Run make: `make -j2`.
* Run the executable: `./tsp_example`.

### Structure

The `Graph` class (`src/graph.h`) is a minimal description of a graph; it only stores the number of nodes and the distance matrix. The graph is created randomly, with nodes lying on a plane and euclidean distances. The `Solver` class does all the CPLEX magic: it builds the model, it solves it, and it prints the solution.