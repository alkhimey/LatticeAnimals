#ifndef _COUNTING_ALGORITHM_H_
#define _COUNTING_ALGORITHM_H_

#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
#include "basic_types.h"
#include "lattice_animal.h"
#include "logging.h"
#include "reserved_queue.h"

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

template <class A, class C, dim_t D>
void redelemeier_main (coord_t n,
		       //dim_t d,
		       coord_t n0,
		       count_t low_id,
		       count_t  high_id,
		       std::vector<count_t>* results,
		       std::ofstream* dump_file)
{

  clock_t timer = clock();
  C counter(n, D);
  A a(D,n);
  a.add(a.get_origin()); // add the origin
  counter.increment(a.size(), a.get_count());
  count_t curr_id = 0;
  ReservedQueue untried_queue;
  a.get_new_untried(&untried_queue);


  redelemeier_recursive(a,
			&untried_queue,
			n0,
			low_id, 
			high_id,
			curr_id,
			counter,
			dump_file); 

  a.pop();

  // Write results to the logger
  counter.output_to_log();


  // Return results in raw form
  // counter.output_to_vector(results);
  double total_time = (clock() - timer) / ((float) CLOCKS_PER_SEC);
  std::cout << "total time: " << total_time << std::endl;

}


template <class A, class C>
count_t redelemeier_recursive(A &a,
			      ReservedQueue* untried_queue,
			      coord_t n0,
			      count_t low_id,
			      count_t  high_id,
			      count_t curr_id, // current id of animals of size n0
			      C &counter,
			      std::ofstream* dump_file) {

  static std::vector<ReservedQueue> queue_stack(100);
  static size_t queue_stack_index = 0;
  static double recursion_counter = 0;
  static int num_incs = 0;

  while(!untried_queue->IsEmpty()) {

    index_t c = untried_queue->Top();
    untried_queue->Pop();
    // Cannot add cell into the new lattice animal only if the lattice animal is in the counted class.
    if (!a.can_add(c)) {
      std::cout << "cant add\n";
      exit(1);
      continue;
    }
    int a_size;
    int a_count;
    if (a.is_almost_full()) {
      a_size = a.size() + 1;
      a_count = a.perim_when_added(c);
    } else {
      // Add cell into the new lattice animal only if
      // the lattice animal is in the counted class.
      a.add(c);
      a_size = a.size();
      a_count = a.get_count();
      // Count only those which are in the search range

      if (unlikely(queue_stack.size() == queue_stack_index)) {
        queue_stack.resize(queue_stack.size() * 2);
      }
      ReservedQueue* untried_next = &queue_stack[queue_stack_index];
      ++queue_stack_index;
      untried_next->Clear();
      untried_next->CopyQueue(*untried_queue);

      // add neighbours of c to the untried set
      // will add only neighbours which are not neighbours of p

      a.get_new_untried(untried_next);

      curr_id = redelemeier_recursive(a,
       				        untried_next,
                		  n0,
                		  low_id,
                		  high_id,
                		  curr_id,
                		  counter,
                		  dump_file);
    --queue_stack_index;
      a.pop();
    }
    counter.increment(a_size, a_count);
  }
  if (a.size() == 1) {
    std::cout << "recursion time: " << recursion_counter << std::endl;
    std::cout << "num_incs: " << num_incs << std::endl;

  }

  return curr_id;
}
			   
			   
							




#endif
