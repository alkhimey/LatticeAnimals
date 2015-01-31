#include <stack>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include <map>
#include <limits.h>
#include <assert.h>

#include <iostream>

using namespace std;

/**
 * Improved version of the line convex counting algorithm family.
 *
 */
namespace line_convex_multi_dim_v2 {

  enum Dim3d { X, Y, Z, LAST_DIM };
  
  /**
   * Maximum number of cells in 1D stick. Big enough to avoid out of range checks for any N we will use.
   */
  enum { MAX_COORD = 20 }; // TODO: Reset to large number after debugging 
  enum { ORIGIN_COORD = MAX_COORD / 2 };

  class Cell;
  class ConvexPolycube;
  class CompareCells;

  typedef unsigned long long count_t;
  typedef int coord_t;
  typedef Dim3d dim_t;
  typedef priority_queue<Cell, vector<Cell>, CompareCells> UntriedSet; 

  class Cell {
  private:
    coord_t _c[LAST_DIM];
    
  public:
    Cell(coord_t x, coord_t y, coord_t z) {
      _c[X] = x;
      _c[Y] = y;
      _c[Z] = z;
    };
    
    bool operator== (const Cell &other) const {
      return (_c[X] == other._c[X] &&
	      _c[Y] == other._c[X] &&
	      _c[Z] == other._c[X]);
    }
    
    bool operator!= (const Cell &other) const {
      return !(*this == other);
    }

    bool operator< (const Cell &other) const {

      for(int d = X; d < LAST_DIM; d++) {
	if(_c[d] < other._c[d]) {
	  return true;
	} else if(_c[d] > other._c[d]) {
	  return false;
	}
      }

      return false;
    }

    coord_t &operator[](dim_t d) {
      return _c[d];
    }
  };
    

  /// Used to create a min-heap
  struct CompareCells{
    bool operator()(Cell lhs, Cell rhs) const {
      return rhs < lhs;
    }
  } ;

  class ConvexPolycube {
  private:
       
    
    bool _p[MAX_COORD][MAX_COORD][MAX_COORD];
    bool _not_empty[LAST_DIM][MAX_COORD][MAX_COORD]; // is row not empty 

    unsigned int _size;
    
  public:
    ConvexPolycube(Cell origin) {
      
      for (int ix = 0; ix < MAX_COORD; ix++) {
	for (int iy = 0; iy < MAX_COORD; iy++){
	  for (int iz = 0; iz < MAX_COORD; iz++){
	    _p[ix][iy][iz] = false;
	  }
	
	  _not_empty[X][ix][iy] = false;
	  _not_empty[Y][ix][iy] = false;
	  _not_empty[Z][ix][iy] = false;
	}
      }

      _p[origin[X]][origin[Y]][origin[Z]] = true;

      _not_empty[X][origin[Y]][origin[Z]] = true;
      _not_empty[Y][origin[X]][origin[Z]] = true;
      _not_empty[Z][origin[X]][origin[Y]] = true;

      _size = 1;
    }
    
    /** 
     * Add cell from polycube. Will add only if convexity is perserved.
     * Returns true if cell was added, false if not.
     */
    bool add(Cell c) {
      assert(_p[c[X]][c[Y]][c[Z]] == false);

      if ((_p[c[X] + 1][c[Y]][c[Z]] ||
           _p[c[X] - 1][c[Y]][c[Z]] ||
      	   _not_empty[X][c[Y]][c[Z]] == false) &&
	  
      	  (_p[c[X]][c[Y] + 1][c[Z]] ||
      	   _p[c[X]][c[Y] - 1][c[Z]] ||
      	   _not_empty[Y][c[X]][c[Z]] == false) &&
	  
      	  (_p[c[X]][c[Y]][c[Z] + 1] ||
      	   _p[c[X]][c[Y]][c[Z] - 1] ||
      	   _not_empty[Z][c[X]][c[Y]] == false)) {

	// Debug-check if it is convex
	if (debug_test_convexity(c) != true) {
	  debug_print_p();
	  assert(false);
	}

	_p[c[X]][c[Y]][c[Z]] = true;
	
	_not_empty[X][c[Y]][c[Z]] = true;
	_not_empty[Y][c[X]][c[Z]] = true;
	_not_empty[Z][c[X]][c[Y]] = true;

	_size++;
	return true;
      }

      // Debug-check if it is not convex
      if (debug_test_convexity(c) != false){
	debug_print_p();
	assert(false);
      }




      return false;
    }
    
    /** 
     * Removes cell from polycube.
     */
    void remove(Cell c) {
      assert(_p[c[X]][c[Y]][c[Z]] == true);
      
      if (_p[c[X] + 1][c[Y]][c[Z]] == false && _p[c[X] - 1][c[Y]][c[Z]] == false) {
	_not_empty[X][c[Y]][c[Z]] = false;
      }

      if (_p[c[X]][c[Y] + 1][c[Z]] == false && _p[c[X]][c[Y] - 1][c[Z]] == false) {
	_not_empty[Y][c[X]][c[Z]] = false;
      }
	  
      if (_p[c[X]][c[Y]][c[Z] + 1] == false &&  _p[c[X]][c[Y]][c[Z] - 1] == false) {
	_not_empty[Z][c[X]][c[Y]] = false;
      }


      _size--;
      _p[c[X]][c[Y]][c[Z]] = false;
    }
      
    /** 
     * Returns true if the cell is adjucent to the current polycube. 
     * Ignoring the provided cell. 
     */
    inline bool is_neigh(Cell c, Cell ignore)  {
      assert( _p[ignore[X]][ignore[Y]][ignore[Z]] == true);

      _p[ignore[X]][ignore[Y]][ignore[Z]] = false;
      
      bool res =  
	_p[c[X] + 1][c[Y]][c[Z]] ||
	_p[c[X] - 1][c[Y]][c[Z]] ||
	_p[c[X]][c[Y] + 1][c[Z]] ||
	_p[c[X]][c[Y] - 1][c[Z]] ||
	_p[c[X]][c[Y]][c[Z] + 1] ||
	_p[c[X]][c[Y]][c[Z] - 1];

      _p[ignore[X]][ignore[Y]][ignore[Z]] = true;
      return res;
    }

    /** 
     * Returns true if the cell is part of the polycube
     */
    inline bool is_contained(Cell c) const {
      return _p[c[X]][c[Y]][c[Z]];
    }

    inline unsigned int size() const {
      return _size;
    }


    void debug_print_not_empty(Dim3d D) const {
      for (int ix = 0; ix < MAX_COORD; ix++) {
	for (int iy = 0; iy < MAX_COORD; iy++) {
	  cout << _not_empty[D][ix][iy];
	}
	cout << endl;
      }
    }

    bool debug_is_empty() const {
      bool ret = false;
      for (int ix = 0; ix < MAX_COORD; ix++) {
	for (int iy = 0; iy < MAX_COORD; iy++) {
	  for (int iz = 0; iz < MAX_COORD; iz++) {
	    if(_p[ix][iy][iz] == true) {
	      cout << "Non empty cell at " << ix << " " << iy << " " << iz << endl;
	      ret = true;
	    }
	  }
	}
      }
      return ret;
    }
    
    void debug_print_p() const {
      for (int i = 0; i < MAX_COORD; i++) {
	for(int j = 0; j < MAX_COORD; j++) {
	  for(int k = 0; k < MAX_COORD; k++) {
	    if (_p[i][j][k] == true) {
	      cout << "(" << i << ", " << j << ", " << k << ")";
	    }
	  }
	}
      }
      cout << endl;

    }

    bool debug_test_convexity(Cell c) {
      assert(_p[c[X]][c[Y]][c[Z]] == false);
      _p[c[X]][c[Y]][c[Z]] = true;
      
      bool ret = true;

      for (int i = 0; i < MAX_COORD; i++) {
	for(int j = 0; j < MAX_COORD; j++) {
	  bool start[LAST_DIM] = {false, false, false};
	  bool end[LAST_DIM] = {false, false, false};
	  for(int k = 0; k < MAX_COORD; k++) {

	    if (_p[k][i][j] == true) {
	      if (end[X] == false) {
		start[X] = true;
	      } else {
		ret =  false;
	      }      
	    } else {
	      if (start[X] == true) {
		end[X] = true;
	      }
	    }

	    if (_p[i][k][j] == true) {
	      if (end[Y] == false) {
		start[Y] = true;
	      } else {
		ret =  false;
	      }      
	    } else {
	      if (start[Y] == true) {
		end[Y] = true;
	      }
	    }

	    if (_p[i][j][k] == true) {
	      if (end[Z] == false) {
		start[Z] = true;
	      } else {
		ret =  false;
	      }      
	    } else {
	      if (start[Z] == true) {
		end[Z] = true;
	      }
	    }
	  }	  
	}
      }

      _p[c[X]][c[Y]][c[Z]] = false;
      return ret;
    } // debug_test_convexity
    

    

  };

  


  /**
   * Will add neighbours of the cell to the untried set u, which are not neighbours of
   * the polycube p or part of it. 
   *
   */
  void add_new_neig_to_untried(Cell& _c, UntriedSet* u, ConvexPolycube* p) {
    vector<Cell> candidates;
       


    candidates.push_back(Cell(_c[X] + 1, _c[Y],     _c[Z]));
    candidates.push_back(Cell(_c[X],     _c[Y] + 1, _c[Z]));     
    candidates.push_back(Cell(_c[X],     _c[Y],     _c[Z] + 1));
    
    if(!(_c[Z] == ORIGIN_COORD && ((_c[X] == ORIGIN_COORD &&  _c[Y] >= ORIGIN_COORD) || (_c[X] == (ORIGIN_COORD + 1) &&  _c[Y] < ORIGIN_COORD)))) {
      candidates.push_back(Cell(_c[X] - 1, _c[Y],     _c[Z]));
    }
    
    if(!(_c[X] == ORIGIN_COORD &&  _c[Y] == ORIGIN_COORD &&  _c[Z] == ORIGIN_COORD)) {
      candidates.push_back(Cell(_c[X],     _c[Y] - 1, _c[Z]));
    }
    
    if(_c[Z] > ORIGIN_COORD + 1 || 
       (_c[Z] == ORIGIN_COORD + 1 && _c[X] >= ORIGIN_COORD &&  _c[Y] >= ORIGIN_COORD) || 
       (_c[Z] == ORIGIN_COORD + 1 && _c[X] >= ORIGIN_COORD + 1 && _c[Y] < ORIGIN_COORD)) {
      candidates.push_back(Cell(_c[X],     _c[Y],     _c[Z] - 1));
    }  
    
    
    for (vector<Cell>::iterator iter=candidates.begin(); iter != candidates.end(); ++iter) {
      if (p->is_neigh(*iter, _c) == false && p->is_contained(*iter) == false) {
	u->push(*iter);
      }
    }
  }




  void redelemeier_recursive(ConvexPolycube& p, 
			     UntriedSet& untried, 
			     unsigned int n,
			     unsigned int n0, 
			     count_t &curr_id, 
			     count_t low_id, 
			     count_t hight_id,
			     vector<count_t>* results) {
   
	
    while(untried.empty() == false) {
      
      if(curr_id >= hight_id)
	return;
      
      Cell c = untried.top();
      untried.pop();

      // add will fail if the new polycube is not convex
      if (p.add(c) == false) {
	continue;
      }
      
      // count only polyominoes that are in the search range
      if(p.size() >= n0 && curr_id >= low_id && curr_id < hight_id)
	(*results)[p.size()]++;
      
      if(p.size() < n0 || (p.size() >= n0 && p.size() < n && curr_id >= low_id && curr_id < hight_id) ) {
	UntriedSet untried_next = UntriedSet(untried);

	// add neighbours of c to the untried set
	// will add only neghbours whcihc are not negibours of p
	add_new_neig_to_untried(c, &untried_next, &p); 


	redelemeier_recursive(p, untried_next, n, n0, curr_id, low_id, hight_id, results);
      }
		
      if(p.size() == n0) 
	curr_id++;
      
      p.remove(c);
    }
  }
  

  



  /**
   * Algorithm entry point
   */
  void line_convex_counter_3d(unsigned int n,
			      unsigned int n0,
			      count_t low_id,
			      count_t hight_id,
			      vector<count_t>* results) {
    Cell origin(ORIGIN_COORD, ORIGIN_COORD, ORIGIN_COORD);

    ConvexPolycube p(origin);


    UntriedSet untried;
    add_new_neig_to_untried(origin, &untried, &p); 
    
    count_t curr_id = 0;
  
    redelemeier_recursive(p, 
			  untried, 
			  n,
			  n0, 
			  curr_id, 
			  low_id, 
			  hight_id,
			  results); 
    p.remove(origin);

    /*  p.debug_print_not_empty(X);
    cout << "*\t*\t*\t*\t**"<<endl;
    p.debug_print_not_empty(Y);
    cout << "*\t*\t*\t*\t**"<<endl;
    p.debug_print_not_empty(Z);
    cout << p.debug_is_empty() << endl;
    */	
    
    
  }



};



