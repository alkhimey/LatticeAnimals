#ifndef _COUNTING_ALGORITHM_H_
#define _COUNTING_ALGORITHM_H_

#include <vector>

typedef unsigned long long int count_t;

/* Prototype for a counting algorthim entry point 
 * 
 * @param n		The maximum size of the polyominoes to count
 * @param n0		
 * @param low_id	
 * @param hight_id	
 * @param results 	The results of the count stored by size. Relevant only for sizes between n0 and n.
 */ 
typedef void (*CountingAlgorithm)(unsigned int n,
				  unsigned int n0,
				  count_t lowId,
				  count_t hightId,
				  std::vector<count_t>* results);



#endif
