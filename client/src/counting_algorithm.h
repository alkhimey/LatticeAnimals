#ifndef _COUNTING_ALGORITHM_H_
#define _COUNTING_ALGORITHM_H_

#include <vector>
#include <list>
#include <fstream>
#include "basic_types.h"
#include "lattice_animal.h"

/* Prototype for a counting algorthim entry point 
 * 
 * @param n		The maximum size of the lattice animal to count
 * @param n0		
 * @param low_id	
 * @param high_id	
 * @param results 	The results of the count stored by size. Relevant only for sizes between n0 and n.
 * @param dump_file     Pointer to the dump stream. The stream should be already open for writing. If stream is
 *                      open, the algortihm should dump all the polycubes that it has counted.
 */ 
typedef void (*CountingAlgorithm) (coord_t n,
				   // dim_t d,
				   coord_t n0,
				   count_t low_id,
				   count_t  high_id,
				   std::vector<count_t>* results,
				   std::ofstream* dump_file);


template <class A, dim_t D>
void redelemeier_main (coord_t n,
		       //dim_t d,
		       coord_t n0,
		       count_t low_id,
		       count_t  high_id,
		       std::vector<count_t>* results,
		       std::ofstream* dump_file)
{

  A a(D,n);
  a.add(a.get_origin()); // add the origin
    
  count_t curr_id = 0;
  std::list< index_t > untried = a.get_new_untried();
  (*results)[1] = 1;

  redelemeier_recursive(a,
			untried,
			n0,
			low_id, 
			high_id,
			curr_id,
			results,
			dump_file); 

  a.pop();
}





template <class A>
count_t redelemeier_recursive(A &a,
			      std::list< index_t > &untried,
			      coord_t n0,
			      count_t low_id,
			      count_t  high_id,
			      count_t curr_id, // current id of animals of size n0
			      std::vector<count_t>* results,
			      std::ofstream* dump_file) {

  
  std::list< index_t >::iterator iter = untried.begin();
  


  while(iter != untried.end()) {      

    if(curr_id >= high_id) {
      return curr_id;
    }
    
    index_t c = *iter;
    
    // Add cell into the new lattice animal only if the lattice animal is in the counted class.
    if (a.can_add(c)) {

      a.add(c);
      iter = untried.erase(iter);
      
    } else {
      
      iter++;
      continue;
      
    }
    
    // Count only those which are in the search range
    if(a.size() >= n0 && curr_id >= low_id && curr_id < high_id && a.is_in_class()) {
      (*results)[a.size()]++;

      if (dump_file != NULL) {
	a.dump(dump_file);
      }

    }
    
    if(a.size() < n0 || 
       (a.size() >= n0       && 
	a.is_full() == false && 
	curr_id >= low_id    && 
	curr_id < high_id) ) {
      
      std::list< index_t > untried_next = std::list< index_t >(untried);
      
      // add neighbours of c to the untried set
      // will add only neghbours which are not negibours of p
      
      untried_next.splice(untried_next.end(),  a.get_new_untried() );
      
      curr_id = redelemeier_recursive(a,
				      untried_next,
				      n0,
				      low_id, 
				      high_id,
				      curr_id,
				      results,
				      dump_file); 
    }
    

    if(a.size() == n0) 
      curr_id++;
    
    a.pop();
  }

  return curr_id;
}
			   
			   
							




#endif
