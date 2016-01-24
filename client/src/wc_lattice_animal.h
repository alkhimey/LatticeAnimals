#ifndef WC_LATTICE_ANIMAL_H_
#define WC_LATTICE_ANIMAL_H_

#include <vector>
#include <list>
#include <utility>
#include <math.h>
#include <assert.h>
#include <fstream>

#include "lattice_animal.h"





/**
 * This class represents a weakly convex lattice animal on a d-dimensional lattice of maximal size n.
 */
class WeaklyConvexLatticeAnimal : public LatticeAnimal {
  

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
  
 WeaklyConvexLatticeAnimal(dim_t d, coord_t n) : LatticeAnimal(d, n)  {
  
    ColumnDataType empty_column;
    empty_column.is_empty = true;

    _columns = std::vector< ColumnDataType >( _d * pow_nd( _dim_size, _d-1), empty_column);

    _index_to_cols = std::vector < std::vector< std::pair< column_t, coord_t > > >(_lattice.size(), std::vector< std::pair< column_t, coord_t > >(_d));
    
    for(index_t idx = 0; idx < _lattice.size(); idx++) {
      
      std::vector<coord_t> coords = _index_to_coord[idx];
      
      for(dim_t i = 0; i < _d; i++) {
	_index_to_cols[idx][i].second = coords[i];
	
	column_t col_idx = 0;
	dim_t offset = 0;
	 for(dim_t j = 0; j < _d; j++) {
	   if (j == i) {
	     offset = -1;
	   } else  { 
	     col_idx += pow_nd(_dim_size, j + offset) * (coords[j] + (_n - 1));
	   }
	 }

	 _index_to_cols[idx][i].first = i * pow_nd(_dim_size, _d - 1) +  col_idx;
      } 
    }
  }    
  
  /**
   * {@inheritDoc} 
   */
  bool can_add(index_t idx) {
    
    if (LatticeAnimal::can_add(idx) == false) {
      return false;
    }
    
    for(std::vector< std::pair< column_t, coord_t > >::iterator iter = _index_to_cols[idx].begin();
	iter != _index_to_cols[idx].end();
	iter++) {
      
      if (_columns[(*iter).first].is_empty == false && 
	  _columns[(*iter).first].low  - 1 != (*iter).second && 
	  _columns[(*iter).first].high + 1 != (*iter).second) {
	return false;
      } 
    }
    
    return true;
  }

  /**
   * {@inheritDoc} 
   */
  void pop() {

    index_t idx = _stack.back();

    for(std::vector< std::pair< column_t, coord_t > >::iterator iter = _index_to_cols[idx].begin(); 
	iter != _index_to_cols[idx].end(); 
	iter++) {   
      
      assert(_columns[(*iter).first].low <= (*iter).second);
      assert(_columns[(*iter).first].high >= (*iter).second);

      if (_columns[(*iter).first].low == _columns[(*iter).first].high) {

	_columns[(*iter).first].is_empty = true;
	
      } else if (_columns[(*iter).first].low == (*iter).second) {
	
	_columns[(*iter).first].low += 1;
	
      } else if (_columns[(*iter).first].high == (*iter).second) {
	
	_columns[(*iter).first].high -= 1;
	
      }
    }

    // Call parent's pop, in reverse order of addition.
    LatticeAnimal::pop();

  }
  
  /**
   * {@inheritDoc} 
   */
  void add(index_t idx) {
    
    LatticeAnimal::add(idx);
    
    for(std::vector< std::pair< column_t, coord_t > >::iterator iter = _index_to_cols[idx].begin(); 
	iter != _index_to_cols[idx].end(); 
	iter++) {   

      if (_columns[(*iter).first].is_empty == true) {

	_columns[(*iter).first].is_empty = false;
	_columns[(*iter).first].low = (*iter).second;
	_columns[(*iter).first].high = (*iter).second;

      } else if (_columns[(*iter).first].low - 1 == (*iter).second) {

	_columns[(*iter).first].low -= 1;

      } else if (_columns[(*iter).first].high + 1 == (*iter).second) {

	_columns[(*iter).first].high += 1;

      } else {
	assert(false);
      }
    }
  }


  /**
   * {@inheritDoc} 
   */
  //void add_origin() {
  //  std::vector<coord_t> c(_d, 0);
  //  add(calc_index(c));
  //}

  
};





#endif
