#ifndef LATTICE_ANIMAL_H_
#define LATTICE_ANIMAL_H_

#include <vector>
#include <list>
#include <utility>
#include <math.h>
#include <assert.h>

#include "basic_types.h"



/**
 * Power utility function. Calculates n**d.
 */
index_t pow_nd(coord_t n, dim_t d);


/**
 * This class represents a lattice animal on a d-dimensional lattice of maximal size n.
 */
class LatticeAnimal {
  
  // TODO: Artium remove!!!!!
 public:
  std::vector< index_t >           get_stack() const {
    return _stack;
  }


protected:
  dim_t _d;
  coord_t _n;
  coord_t _dim_size;
  
  std::vector<bool>                _lattice;          /// size of the lattice is n**d

  std::vector< index_t >           _stack;

  
  /** 
   * Precomputed translation table from index to a vector of coordinates.
   */
  std::vector< std::vector< coord_t > > _index_to_coord; 

  /**
   *  Precomputed lists of neighbours for each cell. 
   *  Notice that it takes into account that our lattice has some "forbidden" cells.
   */
  std::vector< std::list< index_t > >    _neighbours;     

  /**
   * Inner add function. It should be hidden by specialised lattice animal classes that 
   * inherit from this class. 
   *
   * @param  idx Reuse of the index that was calculated out of c.
   * @return     True is the cell is added. False if the cell is rejected and not added.
   *             The public add function acts upon this value.
   */
  bool inner_add(index_t idx);

public:
  
  /** 
   * Class constructor.
   * The size of the lattice is equal to (2*(n-1)+1) ^ d
   *
   * @param  d Dimension of the lattice 
   * @param  n Upper limit to the size of the animal (maximum number of cells).
   */
 LatticeAnimal(dim_t d, coord_t n)  {
    assert(d > 0);
    assert(n > 0);

    _d = d;   
    _n = n;
    _dim_size =  2 * _n -1;

    _lattice = std::vector<bool>( pow_nd( _dim_size , _d ), false );

    _index_to_coord = std::vector< std::vector< coord_t > >(_lattice.size());
    _neighbours     =  std::vector< std::list< index_t > >(_lattice.size());

    // Precompute index to ccordinate list translation table
    //
    for(index_t idx = 0; idx < _lattice.size(); idx++) {
      
      std::vector< coord_t > c = calc_coordinates(idx);

      _index_to_coord[idx] = c;
    }

    // Precompute index to list of neighbours translation table
    //
    for(index_t idx = 0; idx < _lattice.size(); idx++) {

      std::vector< coord_t > c = _index_to_coord[idx];
 
      std::list< index_t > neigh;
      for(dim_t dim = 0; dim < c.size(); dim++) {

	// -1, +1
	for (int offset = -1; offset <= 1; offset+=2) {
	  std::vector< coord_t > c2 = c;


	  // Notice that cells at the edge have neighbours beyond the edge. This is ok.
	  c2[dim] += offset;

	  if(is_valid_cell(c2)) {

	    neigh.push_back(calc_index(c2));
	  }
	}
      } 
      
      _neighbours[idx] = neigh;
    }
  }  


  /**
   * Checks if the cell is inside the valid region on the lattice.
   * It is sufficient to run this function once when initialising the precomputed neighbours.
   *
   * @param  c A list of coordinates of any dimansion.
   * @return   true if this cell is a valid cell. Invalid cell for the redelemeier 
   *           algorithm are cells whos X coordinate is negative or it is zero
   *           and some other coordinate is negative. 
   *
   */
  static inline bool is_valid_cell( const std::vector< coord_t > c) {

    std::vector< coord_t >::const_iterator iter = c.begin();

    if (*iter < 0) { 
	return false; 
    } else if (*iter == 0) {

      for (iter++; iter < c.end(); iter++ ) {
	if (*iter > 0) { 
	  return true;
	} else if (*iter < 0) {
	  return false;
	}	
      }
    }

   return true;
  }

  /**
   * A d-dimensional lattice is stored in a flat array. This
   * function calculates the index of cell c in this array.
   * This function does not discriminate invalid cells.
   *
   * @param  c Vector containing d coordinates, ordered according to the dimensions.
   * @return   The calculated index
   */
  index_t calc_index(const std::vector<coord_t>& c) const;


  /**
   * A d-dimensional lattice is stored in a flat array. This
   * function calculates the coordinates of the cell stored at location
   * idx in the array.
   *
   * @param  idx The index in the flat array.
   * @param  c   Output vector containing d coordinates, ordered according to the dimensions.
   */
  std::vector<coord_t> calc_coordinates(index_t idx) const;


  /**
   * Adds the origin cell to the lattice animal. 
   */
  void add_origin() {
    std::vector<coord_t> c(_d, 0);
    add(calc_index(c));
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
   * @pre        idx is not contained in the lattice animal. Lattice animal has not reached maximal size.
   * @param  idx Index of the added cellx
   * @return     True is the cell is added. False if the cell is rejected and not added.
   *             The reason of rejection depends on the type of the class this animal 
   *             belongs to. Rejection means that the new animal does not belong to that
   *             class.
   */
  bool add(const index_t idx);

  /**
   * Check if cell the polycube containing the cell c
   *
   * @param  idx Index of the cell in question.
   * @return     True if the cell is part of this lattice aminal.
   */
  //bool is_contained(const index_t idx) const;


  /**
   * Check if cell c adjacent to some cell of this polycube
   *
   * @param  idx Index of the cell in question.
   * @return     True if the cell is adjacent to some cell of this lattice animal.
   */
  //bool is_neigh(const index_t idx) const;


  /**
   * Get a list of cells that are neighbours of the newly added cell but have not been
   * in the undried list before (not neighbours of a cell that is already part of the lattice
   * animal.
   *
   */
  std::list< index_t > get_new_untried() const;

  /** 
   * @return true if the current number of cells of the lattice animall. 
   *         This does NOT return the number of cells allocated for the lattice. 
   */
  coord_t size() const {
    return _stack.size();
  }

  /**
   * @return true if the number of cell of the lattice animal reached the maximum.
   */
  bool is_full() const {
    assert(size() <= _n);
    return size() == _n;
  }
};



#endif
