#include <assert.h>


#include "lattice_animal.h"

/**
 * @TODO: Use lookup table to improve speed
 */
index_t pow_nd(coord_t n, dim_t d) {
  assert(n >= 0);
  assert(d >= 0);
  index_t res = 1;
  for(dim_t i = 0; i < d; i++) {
    res *= n;
  }

  return res;
}


index_t LatticeAnimal::calc_index(const std::vector<coord_t>& c) const
{
  assert(c.size() == _d);

  index_t idx = 0;
  for(dim_t d = 0; d < _d; d++) {
    idx += pow_nd(_dim_size, d) * (c[d] + (_n - 1));
  }

  return idx;
}



std::vector<coord_t> LatticeAnimal::calc_coordinates(index_t idx) const 
{
  std::vector<coord_t> c(_d);

  index_t curr_idx = idx;

  for(dim_t d = 0; d < _d; d++) {
    c[ _d - d - 1]     = curr_idx / pow_nd(_dim_size, _d - d - 1) - (_n - 1);
    curr_idx = curr_idx % pow_nd(_dim_size, _d - d - 1);
  }

  return c;
}



bool LatticeAnimal::inner_add(index_t idx) {
  return true; 
}



bool LatticeAnimal::add(const index_t idx)
{
  assert(is_full() == false);
  assert(_lattice[idx] == false);

  if (inner_add(idx) == true) {

    _stack.push_back(idx);
    _lattice[idx] = true;

    return true;

  } else {

    return false;

  }
}


void LatticeAnimal::pop() 
{
  assert(_stack.size() > 0);

  _lattice[_stack.back()] = false;
  _stack.pop_back();

}

/**
 * @TODO: It is possible to optimize the usage of add especialy if we need sorted list.
 */ 
std::list< index_t > LatticeAnimal::get_new_untried() const
{
  index_t                        last_idx        = _stack.back();


  std::list< index_t >           candidates      = _neighbours[last_idx];
  std::list< index_t >           result;
  std::list< index_t >::iterator candidates_iter;

  for(candidates_iter = candidates.begin(); candidates_iter != candidates.end(); candidates_iter++) {
  
    std::list< index_t >           neighs      = _neighbours[*candidates_iter];
    std::list< index_t >::iterator neigh_iter;

    if (_lattice[*candidates_iter] == false) {

      for(neigh_iter = neighs.begin(); neigh_iter != neighs.end(); neigh_iter++) {
    
	if (_lattice[*neigh_iter] && *neigh_iter != last_idx ) {
	  break;	
	}
      }

      if( neigh_iter == neighs.end()) {
	result.push_back(*candidates_iter);
      }
    }
  }

  return result;
}
