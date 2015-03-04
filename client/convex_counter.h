#include <vector>

#include "counting_algorithm.h"

using namespace std;

namespace line_convex_multi_dim_v2 {

  /** 
   * Improved version that counts convex polycubes in a smart way pruning trees of non convex polycubes
   */
  void line_convex_counter_3d(unsigned int n,
			      unsigned int n0,
			      count_t lowId,
			      count_t hightId,
			      vector<count_t>* results, 
			      std::ofstream* dump_file);
};
