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
class StronglyConvexLatticeAnimal : public LatticeAnimal {
  

protected:
  struct ColumnDataType {
    bool is_empty;
    coord_t low;
    coord_t high;
  };
 
  // Cell index to a vector of std::pairs of column and location in that column
  std::vector < std::vector< std::pair< column_t, coord_t > > > _index_to_cols;

  std::vector< ColumnDataType > _columns;

public:
  
  StronglyConvexlatticeanimal(dim_t d, coord_t n) : WeaklyConvexLatticeAnimal(d, n)  {

 }    


/**
   * Determines if the current lattice animal is strongly convex.
   *
   **/
  bool is_in_class(void) {



    
    return true;
  }
  
  
};





#endif
