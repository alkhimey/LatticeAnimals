#ifndef SC_LATTICE_ANIMAL_H_
#define SC_LATTICE_ANIMAL_H_

#include <vector>
#include <list>
#include <utility>
#include <math.h>
#include <assert.h>
#include <fstream>

#include "lattice_animal.h"
#include "wc_lattice_animal.h"




/**
 * This class represents a strongly convex lattice animal on a d-dimensional lattice of maximal size n.
 */
class StronglyConvexLatticeAnimal : public WeaklyConvexLatticeAnimal {
  

protected:
  struct ColumnDataType {
    bool is_empty;
    coord_t low;
    coord_t high;
  };
 
  // Cell index to a vector of std::pairs of column and location in that column
  std::vector < std::vector< std::pair< column_t, coord_t > > > _index_to_cols;

  std::vector< ColumnDataType > _columns;

  subspace_index_t _number_of_subspaces; /// Total number of subspaces. Calculated in constructor.


  /**
   * Calculates the total number of subspaces of space of dimension d.
   * For example, if d=3 the subspaces are: X, Y, Z, XY, XZ, YZ.
   * Notice that in the example, XYZ is not considered a subspace.
   */
  subspace_index_t calc_number_of_subspaces() const;

  /**
   * Return true if the cell belongs to the subspace. Irregardles of the polyomino itself.
   */
  bool is_cell_in_subspace(index_t idx, subspace_index_t subsp_idx) const;

  /**
   * Perform a BFS on the polymino cells that reside in the subspace.
   * The algorithm starts running from the provided first cell.
   *
   * @return Number of reached cell.
   */
  index_t polyomino_BFS_count(subspace_index_t subsp_idx, index_t first_idx) const;


  
public:
  
  StronglyConvexLatticeAnimal(dim_t d, coord_t n) : WeaklyConvexLatticeAnimal(d, n)  {
    _number_of_subspaces = calc_number_of_subspaces();
 }    






  
  /**
   * Determines if the current lattice animal is strongly convex by checking if the polyomino is connected
   * in every subspace.
   *
   * @algorithm
   *   For each sub-space:
   *     X1 := Count total number of cells in polyomino that are inside this sub-space
   *     X2 := Preform BFS inside this sub-space starting from arbitrary cell and count number of reached cells.
   *     if X1 not equal X2 then return false
   *
   *
   **/
  bool is_in_class(void);


};



#endif
