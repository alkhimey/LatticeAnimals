#include <assert.h>


#include "lattice_animal.h"



index_t LatticeAnimal::get_index(const std::vector<coord_t>& c) 
{
  index_t idx = 0;
  for(dim_t d = 0; d < c.size(); d++) {
    idx += (d + 1) * c[d];
  }

  return idx;
}



bool LatticeAnimal::inner_add(const std::vector<coord_t>& c, index_t idx) {
  return true; // this class is for the general lattice animals (fixed, no holes).
}



bool LatticeAnimal::add(const std::vector<coord_t>& c)
{
  assert(c.size() == _d);

  index_t idx = get_index(c);

  assert(_lattice[idx] == false);

  if (inner_add(c, idx) == true) {

    _cell_stack.push_back(c);
    _lattice[idx] = true;

    return true;

  } else {

    return false;

  }
}


void LatticeAnimal::pop() 
{
  assert(_cell_stack.size() > 0);

  std::vector<coord_t> c = _cell_stack.back();
  _cell_stack.pop_back();

}

bool LatticeAnimal::is_contained(const std::vector<coord_t>& c) const 
{
  return _lattice[get_index(c)];
}


bool LatticeAnimal::is_neigh(std::vector<coord_t>& c) const 
{

  // TODO: This can be optimised (index calculation)
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

