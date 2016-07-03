#ifndef WC_FILTERED_LATTICE_ANIMAL_H_
#define WC_FILTERED_LATTICE_ANIMAL_H_

#include <vector>
#include <list>
#include <utility>
#include <math.h>
#include <assert.h>
#include <fstream>

#include "lattice_animal.h"


#define ___MAX_N 64              // TODO:These constant are temoporary hack because I did not want to invest debugging time in a dynamically allocated solution.
#define ___OFFSET ___MAX_N / 2   //       n is a paraemter to this class, so there is no reason why I can't use it to create arrays of 2*n-1 size.

/**
 * This class represents a weakly convex lattice animal on a d-dimensional lattice of maximal size n.
 */
class WeaklyConvexFilteredLatticeAnimal : public LatticeAnimal {
  

protected:
  struct ColumnDataType {
    coord_t num_of_occupied; /// Number of occupied cells in column
    bool column_data[___MAX_N];
  };
 
  coord_t _num_of_segments;    /// Total number of seperate segments in all columns
  coord_t _num_of_active_cols; /// Number of columns that have at least one occupied cell

  // Cell index to a vector of std::pairs of column and location in that column
  std::vector < std::vector< std::pair< column_t, coord_t > > > _index_to_cols;

  std::vector< ColumnDataType > _columns;

public:
  
 WeaklyConvexFilteredLatticeAnimal(dim_t d, coord_t n) : LatticeAnimal(d, n)  {
  
    ColumnDataType empty_column = { 0, {0} };

    _num_of_segments = 0;
    _num_of_active_cols = 0;
 //   empty_column.num_of_segments = 0;
 //   empty_column.num_of_occupied = 0;
 //   empty_column.column_data = {0};

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
  bool is_in_class(void) {

    return _num_of_segments == _num_of_active_cols;
  }

  /**
   * {@inheritDoc} 
   */
  void pop() {

    index_t idx = _stack.back();

    for(std::vector< std::pair< column_t, coord_t > >::iterator iter = _index_to_cols[idx].begin(); 
	iter != _index_to_cols[idx].end(); 
	iter++) {   

      assert(_columns[(*iter).first].column_data[(*iter).second + ___OFFSET] == true);
      
      if (_columns[(*iter).first].num_of_occupied == 1) { 

        _num_of_segments    -= 1;
        _num_of_active_cols -= 1;

      } else if (_columns[(*iter).first].column_data[(*iter).second - 1 + ___OFFSET] == false  && 
                 _columns[(*iter).first].column_data[(*iter).second + 1 + ___OFFSET] == false){

        _num_of_segments -= 1;

      } else if ( (*iter).second != 0 && 
                  _columns[(*iter).first].column_data[(*iter).second - 1 + ___OFFSET] == true && 
                  _columns[(*iter).first].column_data[(*iter).second + 1 + ___OFFSET] == true) {

        _num_of_segments += 1;

      }
      
      _columns[(*iter).first].num_of_occupied -= 1;
      _columns[(*iter).first].column_data[(*iter).second + ___OFFSET] = false;

      assert(_num_of_segments >= 0);
      assert(_num_of_active_cols >= 0);
    }

    

    // Call parent's pop, in reverse order of addition.
    LatticeAnimal::pop();

  }
  
  /**
   * {@inheritDoc} 
   */
  void add(index_t idx) {
    
    // Call parent's add
    LatticeAnimal::add(idx);
    
    for(std::vector< std::pair< column_t, coord_t > >::iterator iter = _index_to_cols[idx].begin(); 
	iter != _index_to_cols[idx].end(); 
	iter++) {   

      assert(_columns[(*iter).first].column_data[(*iter).second + ___OFFSET] == false);

      if (_columns[(*iter).first].num_of_occupied == 0) {

        _num_of_segments    += 1;
        _num_of_active_cols += 1;

      } else if (_columns[(*iter).first].column_data[(*iter).second - 1 + ___OFFSET] == false  && 
                 _columns[(*iter).first].column_data[(*iter).second + 1 + ___OFFSET] == false) {

        _num_of_segments += 1;

      } else if ( (*iter).second != 0 && 
                  _columns[(*iter).first].column_data[(*iter).second - 1 + ___OFFSET] == true && 
                  _columns[(*iter).first].column_data[(*iter).second + 1 + ___OFFSET] == true) {

        _num_of_segments -= 1;

      }

      _columns[(*iter).first].num_of_occupied += 1;
      _columns[(*iter).first].column_data[(*iter).second + ___OFFSET] = true;


      assert(_num_of_segments >= 0);
      assert(_num_of_active_cols >= 0);
    }

    
  }
  
};





#endif
