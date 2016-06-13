#include "sc_lattice_animal.h"

bool StronglyConvexLatticeAnimal::is_in_class(void)
{
  for( subspace_index_t subsp_idx = 0; subsp_idx < _number_of_subspaces; subsp_idx++) {
    index_t x1 = 0;
    index_t x2 = 0;
    
    for( auto &cell_idx : _stack) {
      if(is_cell_in_subspace(cell_idx, subsp_idx)) {
	x1 = x1 + 1;
      }
    }

    x2 = polyomino_BFS_count(subsp_idx, _stack.front());

    if (x1 != x2) {
      return false;
    }
  }
    
  return true;
}



//inline subspace_index_t StronglyConvexLatticeAnimal::calc_number_of_subspaces() const
subspace_index_t StronglyConvexLatticeAnimal::calc_number_of_subspaces() const
{
  return 42;
}


bool StronglyConvexLatticeAnimal::is_cell_in_subspace(index_t idx, subspace_index_t subsp_idx) const
{
  return true;
}


index_t StronglyConvexLatticeAnimal::polyomino_BFS_count(subspace_index_t subsp_idx, index_t first_idx) const
{
  return 99;
}

