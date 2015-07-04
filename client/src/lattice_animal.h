#ifndef LATTICE_ANIMAL_H_
#define LATTICE_ANIMAL_H_

#include <vector>

#include "basic_types.h"

/**
 * Index for the flat array.
 */
typedef uint32_t index_t;


/**
 * This class represents a lattice animal on a d-dimensional lattice of maximal size n.
 */
class LatticeAnimal {
  
private:
  dim_t _d;
  coord_t _n;
  
  std::vector<bool>                _lattice;          // size of n**d
  //  std::vector<bool>                _not_empty_column; // size of d*n**(d-1) 
  std::vector< std::vector<coord_t> > _cell_stack;

  /**
   * Inner add function. It should be hidden by specialised lattice animal classes that 
   * inherit from this class. 
   *
   * @param  c   Vector containing d coordinates, ordered according to the dimensions.
   * @param  idx Reuse of the index that was calculated out of c.
   * @return     True is the cell is added. False if the cell is rejected and not added.
   *             The public add function acts upon this value.
   */
  bool inner_add(const std::vector<coord_t>& c, index_t idx);

  
public:
  
  
  LatticeAnimal(dim_t d, coord_t n) {
    // TODO: Constructor
    
  }

  /**
   * A d-dimensional lattice is stored in a flat array. This
   * function calculates the index of cell c in this array.
   *
   * @param  d Number of dimensions  
   * @param  c Vector containing d coordinates, ordered according to the dimensions.
   * @return   The calculated index
   */
  static index_t inline get_index(const std::vector<coord_t>& c);
  


  /**
   * Remove the cell that was added last.
   */
  void pop();

  /**
   * Add new cell to the lattice animal. It is assumed that this function will not be called
   * with a cell that is already part of the lattice animal. If this happens in debug version,
   * an assertion will fail. In release version, this will make the program behaviour unexpected.
   *
   * @param  c Vector containing d coordinates, ordered according to the dimensions.
   * @return   True is the cell is added. False if the cell is rejected and not added.
   *           The reason of rejection depends on the type of the class this animal 
   *           belongs to. Rejection means that the new animal does not belong to that
   *           class.
   */
  bool add(const std::vector<coord_t>& c);

  /**
   * Check if cell the polycube containing the cell c
   *
   * @param  c Vector containing d coordinates, ordered according to the dimensions.
   * @return   True if c is part of this lattice aminal.
   */
  inline bool is_contained(const std::vector<coord_t>& c) const;


  /**
   * Check if cell c adjacent to some cell of this polycube
   *
   * @param  c Vector containing d coordinates, ordered according to the dimensions.  
   * @return   True if c is adjacent to some cell of this lattice animal
   */
  inline bool is_neigh(std::vector<coord_t>& c) const;
};


#endif
