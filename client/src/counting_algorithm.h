#ifndef _COUNTING_ALGORITHM_H_
#define _COUNTING_ALGORITHM_H_

#include <vector>
#include <fstream>

typedef unsigned long long int count_t;

/* Prototype for a counting algorthim entry point 
 * 
 * @param n		The maximum size of the polyominoes to count
 * @param n0		
 * @param low_id	
 * @param hight_id	
 * @param results 	The results of the count stored by size. Relevant only for sizes between n0 and n.
 * @param dump_file     Pointer to the dump stream. The stream should be already open for writing. If stream is
 *                      open, the algortihm should dump all the polycubes that it has counted.
 */ 
typedef void (*CountingAlgorithm)(unsigned int n,
				  unsigned int n0,
				  count_t lowId,
				  count_t hightId,
				  std::vector<count_t>* results,
				  std::ofstream* dump_file);



#endif
