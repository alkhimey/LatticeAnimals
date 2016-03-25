#ifndef ADJ_COUNT_LATTICE_ANIMAL_H_
#define ADJ_COUNT_LATTICE_ANIMAL_H_

#include <vector>
#include <list>
#include <utility>
#include <math.h>
#include <assert.h>
#include <fstream>

#include "lattice_animal.h"




/**
 * This lattice animal class is used to count cells adjucent to lattice animals of size n.
 */
class AdjCountLatticeAnimal : public LatticeAnimal {
  

protected:

  count_t _adj_total;
 

  /**
   * Number of negihbours of a cell that is not on the boundary of the lattice.
   */
  const count_t INNER_NUMBER_OF_NEIGH; 

  /**
   * For each cell, stores the number of cell that are part of the lattice and are
   * neighbours of this cell.
   */  
  std::vector<count_t>                _neig_count_lattice; 
  

  /**
   *  Precomputed lists of neighbours for each cell. 
   *  Includes neighboures that are invalid cells. 
   * Does not include neighbours that outside the lattice boundary.
   */
  std::vector< std::list< index_t > >    _comp_neighbours;

public:
  
 AdjCountLatticeAnimal(dim_t d, coord_t n) :  LatticeAnimal(d, n), INNER_NUMBER_OF_NEIGH(2*_d)  {
    
    _adj_total = 0;
    _neig_count_lattice = std::vector<count_t>(_lattice.size(),0);
    _comp_neighbours         =  std::vector< std::list< index_t > >(_lattice.size());

    // precompute index to list of complete neighbours translation table
    for(index_t idx = 0; idx < _lattice.size(); idx++) {

      std::vector< coord_t > c = _index_to_coord[idx];
 
      std::list< index_t > neigh;
      for(dim_t dim = 0; dim < c.size(); dim++) {

	// -1, +1
	for (int offset = -1; offset <= 1; offset+=2) {
	  std::vector< coord_t > c2 = c;

	  c2[dim] += offset;
	  
	  // only neighbours inside the lattice are added
	  if(is_inside_lattice(c2)) {
	    neigh.push_back(calc_index(c2));
	  }
	}
      } 
      
      _comp_neighbours[idx] = neigh;
    }
  }    
  
  /**
   * {@inheritDoc} 
   */
  bool can_add(index_t idx) {  
    return true;
  }

  /**
   * {@inheritDoc} 
   */
  void pop() {

    index_t idx = _stack.back();

    std::list< index_t > c_neighs = _comp_neighbours[idx]; 
    std::list< index_t >::iterator neigh_iter;

    assert(c_neighs.size() >= INNER_NUMBER_OF_NEIGH - 1  ||  _lattice.size() <= 1);

    _adj_total -=  (INNER_NUMBER_OF_NEIGH -  c_neighs.size());

    for(neigh_iter = c_neighs.begin(); neigh_iter != c_neighs.end(); neigh_iter++) {

      _neig_count_lattice[*neigh_iter]--;

      if( _lattice[*neigh_iter] == false &&
	  _neig_count_lattice[*neigh_iter] == 0) {
	
	_adj_total =  _adj_total - 1;
      }
    } 

    // Call parent's pop, in reverse order of addition.
    LatticeAnimal::pop();

    if (_stack.size() != 0) {
      _adj_total = _adj_total + 1;
    } else {
      assert( _adj_total == 0);
    }
  }
  
  /**
   * {@inheritDoc} 
   */
  void add(index_t idx) {

    if (_stack.size() != 0) {
      _adj_total = _adj_total - 1;
    }

    LatticeAnimal::add(idx);
    
    std::list< index_t > c_neighs = _comp_neighbours[idx]; 
    std::list< index_t >::iterator neigh_iter;

    assert(c_neighs.size() >= INNER_NUMBER_OF_NEIGH - 1  ||  _lattice.size() <= 1);

    for(neigh_iter = c_neighs.begin(); neigh_iter != c_neighs.end(); neigh_iter++) {

      if( _lattice[*neigh_iter] == false &&
	  _neig_count_lattice[*neigh_iter] == 0) {
	
	_adj_total =  _adj_total + 1;
      }

      _neig_count_lattice[*neigh_iter]++;

    } 

    /* The lattice has a size dtermined by the largest animal it supposed to hold. 
     * Cells that lay beyond the lattice boundary and adjucent to the animal can not
     * be counted like all the others because _neigh_count_lattice does not contain them.
     * Because such cell can be adjucent to only one lattice animal cell, there is no problem
     * to add/remove them directly to the total counter
     */ 
    _adj_total +=  (INNER_NUMBER_OF_NEIGH -  c_neighs.size());

  }

  
  /**
   * Will return the number of cell adjucent to this lattice animal
   */
  count_t get_count(void) const {
    return _adj_total;
  }

};





#endif
