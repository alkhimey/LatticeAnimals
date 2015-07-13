#ifndef _COUNTING_ALGORITHM_H_
#define _COUNTING_ALGORITHM_H_

#include <vector>
#include <fstream>
#include "basic_types.h"


/* Prototype for a counting algorthim entry point 
 * 
 * @param n		The maximum size of the lattice animal to count
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



typedef void (*CountingAlgorithmXXX)(unsigned int n,
				     unsigned int d,
				     unsigned int n0,
				     count_t lowId,
				     count_t  hightId,
				     std::vector<count_t>* results,
				     std::ofstream* dump_file);



template <class A>
void redelemeier_main (coord_t n,
		       dim_t d,
		       coord_t n0,
		       count_t lowId,
		       count_t  hightId,
		       std::vector<count_t>* results,
		       std::ofstream* dump_file)
{
  A a(d,n);

  std::vector<coord_t> origin;
  for (int i = 0; i < d; i++) {
    origin.push_back(0);
  }

  a.add(origin);
    
  count_t curr_id = 0;
  


  redelemeier_recursive(a,
			n0, 
			curr_id, 
			low_id, 
			hight_id,
			results); 
  p.remove(origin);
}





template <class A>
count_t redelemeier_recursive(A a,
			      list< vector<coord_t> > untried
			      coord_t n0,
			      count_t lowId,
			      count_t  hightId,
			      count_t curr_id,
			      std::vector<count_t>* results,
			      std::ofstream* dump_file) {


  list< vector<coord_t> >::iterator iter = untried.begin();

  while(iter != untried.end()) {      
    
    if(curr_id >= hight_id) {
      return;
    }
    
    Cell c = *iter;
    
    // If the polycube 'p' with addition of cell 'c' becomes non convex, add will not add cell 'c' and return false.
    // The cell will not be removed from the untried list allowing any convex polycubes that are perfixed with
    // 'p' and contain 'c' to be reached through a different path. 
    if (p.add(c) == true) {
      
      iter = untried.erase(iter);
      
    } else {
      
      iter++;
      continue;
      
    }
    
    // Print progress
    static int x4 = 1;
    if(p.size() == 4) {
      cout << "Progress... "  << x4 << "/86" << endl;
      x4++;
    }
    
    // count only polyominoes that are in the search range
    if(p.size() >= n0 && curr_id >= low_id && curr_id < hight_id) {
      p.dump_to_file();
      (*results)[p.size()]++;
    }
    
    if(p.size() < n0 || (p.size() >= n0 && p.size() < n && curr_id >= low_id && curr_id < hight_id) ) {
      UntriedSet untried_next = UntriedSet(untried);
      
      // add neighbours of c to the untried set
      // will add only neghbours which are not negibours of p
      add_new_neig_to_untried(c, &untried_next, &p); 
      
      curr_id = redelemeier_recursive(p, untried_next, n, n0, curr_id, low_id, hight_id, results);
    }
    
    if(p.size() == n0) 
      curr_id++;
    
    p.remove(c);
  }
  

  






}
			   
			   
							














#endif
