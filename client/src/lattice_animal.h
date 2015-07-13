#ifndef LATTICE_ANIMAL_H_
#define LATTICE_ANIMAL_H_

#include <vector>
#include <utility>
#include <math.h>

#include "basic_types.h"

/**
 * Index for the flat array.
 */
typedef uint32_t index_t;


/**
 * Power utility function. Calculates n**d.
 */
index_t pow_nd(coord_t n, dim_t d);


/**
 * This class represents a lattice animal on a d-dimensional lattice of maximal size n.
 */
class LatticeAnimal {
  
private:
  dim_t _d;
  coord_t _n;
  
  std::vector<bool>                _lattice;          /// size of the lattice is n**d

  std::vector< std::pair< index_t > _stack;

  
  std::vector< vector< coord_t > > _index_to_coord;   /// "index to coordinates" constant translation table


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

  
  /**
   * A d-dimensional lattice is stored in a flat array. This
   * function calculates the index of cell c in this array.
   *
   * @param  d Number of dimensions  
   * @param  c Vector containing d coordinates, ordered according to the dimensions.
   * @return   The calculated index
   */
  index_t inline get_index(const std::vector<coord_t>& c) const;


public:
  
  /** 
   * Class constructor
   *
   * @param  d Dimension of the lattice 
   * @param  n Upper limit to the size of the animal (maximum number of cells)
   */
  LatticeAnimal(dim_t d, coord_t n) {
    _d = d;
    _n = n;
    _lattice = std::vector<bool>( pow_nd( 2 * (_n - 1) + 2, _d ), false );
  }  


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
  bool is_contained(const std::vector<coord_t>& c) const;


  /**
   * Check if cell c adjacent to some cell of this polycube
   *
   * @param  c Vector containing d coordinates, ordered according to the dimensions.  
   * @return   True if c is adjacent to some cell of this lattice animal
   */
  bool is_neigh(std::vector<coord_t>& c) const;
};



#endif
