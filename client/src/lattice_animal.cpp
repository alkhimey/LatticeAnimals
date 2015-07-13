#include <assert.h>


#include "lattice_animal.h"


index_t pow_nd(coord_t n, dim_t d) {
  assert(n > 0);
  assert(d > 0);
  index_t res = n;
  for(dim_t i = 0; i < d-1; i++) {
    res *= n;
  }

  return res;
}


index_t LatticeAnimal::get_index(const std::vector<coord_t>& c) const
{
  index_t idx = 0;
  for(dim_t d = 0; d < c.size(); d++) {
    idx += (d + 1) * (c[d] + (_n - 1));
  }

  return idx;
}



bool LatticeAnimal::inner_add(const std::vector<coord_t>& c, index_t idx) {
  return true; 
}



bool LatticeAnimal::add(const std::vector<coord_t>& c)
{
  assert(c.size() == _d);
  assert(_stack.size() <= (std::vector< std::pair< index_t, std::vector<coord_t> > >::size_type)_n);

  index_t idx = get_index(c);

  assert(_lattice[idx] == false);

  if (inner_add(c, idx) == true) {

    _stack.push_back(make_pair(idx, c));
    _lattice[idx] = true;

    return true;

  } else {

    return false;

  }
}


void LatticeAnimal::pop() 
{
  assert(_stack.size() > 0);

  _lattice[_stack.back().first] = false;
  _stack.pop_back();

}

bool LatticeAnimal::is_contained(const std::vector<coord_t>& c) const 
{
  return _lattice[get_index(c)];
}


/**
 *  TODO: This procedure can be optimised using smarter index calculation.
 */
bool LatticeAnimal::is_neigh(std::vector<coord_t>& c) const
{
  for(dim_t d = 0; d < c.size(); d++) { 
    c[d] += 1;
    if (_lattice[ get_index(c) ]) {
      c[d] -= 1;
      return true;
    }

    c[d] -= 2;
    if (_lattice[ get_index(c) ]) {
      c[d] += 1;
      return true;
    }

    c[d] += 1;
  }

  return false;
}

