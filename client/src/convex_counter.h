#include <vector>

#include "counting_algorithm.h"

using namespace std;

namespace redelemeier_with_pruning {

  /** 
   * Improved version that counts convex polycubes in a smart way pruning trees of non convex polycubes.
   */
  void line_convex_counter_3d(unsigned int n,
			      unsigned int n0,
			      count_t lowId,
			      count_t hightId,
			      vector<count_t>* results, 
			      std::ofstream* dump_file);


  /** 
   * Improved version that counts convex polycubes in a smart way pruning trees of non convex polycubes.
   * Counts polycubes according to the full convex defintion
   */
  void full_convex_counter_3d(unsigned int n,
			      unsigned int n0,
			      count_t lowId,
			      count_t hightId,
			      vector<count_t>* results, 
			      std::ofstream* dump_file);


};
