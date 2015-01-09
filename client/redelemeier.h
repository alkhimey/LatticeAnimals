#pragma once

#include <vector>

#include "counting_algorithm.h"

using namespace std;



/* 
 * Simple implementation of the redelemeier counting algortihm of 2d polyominoes
 */
void redelemeier_recursive_3d(unsigned int n,
			      unsigned int n0,
			      count_t lowId,
			      count_t hightId,
			      vector<count_t>* results);


void redelemeier_3d_line_convex(unsigned int n,
				unsigned int n0,
				count_t lowId,
				count_t hightId,
				vector<count_t>* results);

void redelemeier_3d_full_convex(unsigned int n,
				unsigned int n0,
				count_t lowId,
				count_t hightId,
				vector<count_t>* results);


