#include <stack>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <map>
#include <limits.h>


using namespace std;

/**
 * Improved version of the line convex counting algorithm family.
 *
 */
namespace line_convex_multi_dim_v2 {

  enum Dim3d { X, Y, Z, LAST_DIM };
  
  typedef unsigned long long count_t;
  typedef int coord_t;
  typedef Dim3d dim_t 
  
  typedef pair<coord_t, coord_t> range_t;
  typedef coord_t[LAST_DIM] cell_t;
  
  enun State { FREE, UNTRIED, OCCUPIED, DONE };
  
  
  //template <int D>
  class ConvexPolycube {
  private:
    enum { MAX_COORD = 40 }; /// Maximum number of cells in 1D stick
    enum { COORD_OFFSET = 20 }; /// coordinate 0 is stored in this licataion in the arrays
    
    
    range_t ranges[LAST_DIM][MAX_COORD][MAX_COORD];
    

    bool _p[MAX_COORD][MAX_COORD][MAX_COORD];
    
    
  public:
    ConvexPolycube() {
      _p = {0};
    }
    
    bool add(cell_t c) {
      
    }
    
    void remove(cell_t c) {
       range_t r;

      r = ranges[X][c[Y]][c[Z]];
      if(c[X] = r->second) {
	r->second = r->second + 1;
      } else if(c[X] = r->first) {
	r-> first = r->first - 1;
      } else {
	return false;
      }
      ranges[X][c[Y]][c[Z]] = r;


      r = ranges[Y][c[X]][c[Z]];
      if(c[Y] = r->second) {
	r->second = r->second + 1;
      } else if(c[Y] = r->first) {
	r-> first = r->first - 1;
      } else {
	return false;
      }
      ranges[Y][c[X]][c[Z]] = r;


      r = ranges[Z][c[X]][c[Y]];
      if(c[Z] = r->second) {
	r->second = r->second + 1;
      } else if(c[Z] = r->first) {
	r-> first = r->first - 1;
      } else {
	return false;
      }
      ranges[Z][c[X]][c[Y]] = r;
      
      return true;
    }
      
    /** 
     * Returns truw if the cell is adjucent to the current polycube
     */
    bool test_neigh(cell_t c) {
      
    }

  };



};



