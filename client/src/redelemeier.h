#include <vector>
#include <fstream>

#include "counting_algorithm.h"

using namespace std;



/* 
 * Simple implementation of the redelemeier counting algortihm of 2d polyominoes
 */
void redelemeier_recursive_3d(coord_t n,
			      coord_t n0,
			      count_t lowId,
			      count_t hightId,
			      std::map< std::string, count_t >* results,
			      std::ofstream* dump_file);


void redelemeier_3d_line_convex(coord_t n,
				coord_t n0,
				count_t lowId,
				count_t hightId,
				std::map< std::string, count_t >* results,
				std::ofstream* dump_file);

void redelemeier_3d_full_convex(coord_t n,
				coord_t n0,
				count_t lowId,
				count_t hightId,
				std::map< std::string, count_t >* results,
				std::ofstream* dump_file);


