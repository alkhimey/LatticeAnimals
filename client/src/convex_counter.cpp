#include <stack>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include <map>
#include <limits.h>
#include <assert.h>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <list>

#include <iostream>


using namespace std;

/**
 * Improved version of the line convex counting algorithm family.
 *
 */
namespace redelemeier_with_pruning{

  enum Dim3d { X, Y, Z, LAST_DIM };
  
  /**
   * Maximum number of cells in 1D stick. Big enough to avoid out of range checks for any N we will use.
   */
  enum { MAX_COORD = 40 }; // THIS IMPLEMENTATION CAN COUNT UP TO N=19 !
  enum { ORIGIN_COORD = MAX_COORD / 2 };

  class Cell;
  class ConvexPolycube;
  class CompareCells;
  
  typedef list<Cell> UntriedSet;

  typedef unsigned long long count_t;
  typedef int coord_t;
  typedef Dim3d dim_t;
  

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
      int s = 0;
      
      for(int d = X; d < LAST_DIM; d++) {
	s += abs(ORIGIN_COORD - _c[d]);
	s -= abs(ORIGIN_COORD - other._c[d]);
      }

      return s < 0;
    }


    coord_t &operator[](dim_t d) {
      return _c[d];
    }

    coord_t &operator[](unsigned int d) {
      assert(d >= X && d < LAST_DIM);
      return _c[d];
    }


  };
    

  /** 
   * Compare cells using absolute distance from origin
   */
  struct CompareCells{
    bool operator()(Cell lhs, Cell rhs) const {
      return rhs < lhs;
    }
  } ;

  /**
   * Used for debugging, lexicographical order.
   */
  struct CompareCellsLexi{
    bool operator()(Cell a, Cell b) const {
      
      if (a[X] == b[X] && a[Y] == b[Y] && a[Z] < b[Z])	return true; 
      if (a[X] == b[X] && a[Y] < b[Y])		       	return true;
      if (a[X] < b[X])				       	return true;
      return false;   
    }
  } ;
  

  class ConvexPolycube {
  private:
       
    // Used for keeping track of connectivity of rows and slices
    bool _p[MAX_COORD][MAX_COORD][MAX_COORD];
    bool _not_empty[LAST_DIM][MAX_COORD][MAX_COORD]; // is row not empty 

    bool _not_empty_2d_slice[LAST_DIM][MAX_COORD]; // is 2d slice empty

    
    unsigned int _size;

    bool _is_strong_convex;


    // Used for debugging

    coord_t _min[LAST_DIM];
    coord_t _max[LAST_DIM];
    
    set<Cell, CompareCellsLexi> _p_list;



    ofstream* _f;
    
  public:
    ConvexPolycube(Cell origin, ofstream* dump_file, bool is_strong_convex = false) {
      
      for (int ix = 0; ix < MAX_COORD; ix++) {
	for (int iy = 0; iy < MAX_COORD; iy++){
	  for (int iz = 0; iz < MAX_COORD; iz++){
	    _p[ix][iy][iz] = false;
	  }
	
	  _not_empty[X][ix][iy] = false;
	  _not_empty[Y][ix][iy] = false;
	  _not_empty[Z][ix][iy] = false;

	}

	_not_empty_2d_slice[X][ix] = false;
	_not_empty_2d_slice[Y][ix] = false;
	_not_empty_2d_slice[Z][ix] = false;
		  

      }

      _is_strong_convex = is_strong_convex;

      _p[origin[X]][origin[Y]][origin[Z]] = true;

      _not_empty[X][origin[Y]][origin[Z]] = true;
      _not_empty[Y][origin[X]][origin[Z]] = true;
      _not_empty[Z][origin[X]][origin[Y]] = true;

      _not_empty_2d_slice[X][origin[X]] = true;
      _not_empty_2d_slice[Y][origin[Y]] = true;
      _not_empty_2d_slice[Z][origin[Z]] = true;

      

      _size = 1;

      for(int d = X; d < LAST_DIM; d++) {
	_min[d] = origin[d];
	_max[d] = origin[d];
      }

      _p_list.insert(origin);


      _f = dump_file;
    }
    
    /** 
     * Add cell from polycube. Will add only if convexity is perserved.
     * Returns true if cell was added, false if not.
     */
    bool add(Cell c) {
      assert(_p[c[X]][c[Y]][c[Z]] == false);

      assert(c[X] < MAX_COORD);
      assert(c[X] >= 0);

      assert(c[Y] < MAX_COORD);
      assert(c[Y] >= 0);

      assert(c[Z] < MAX_COORD);
      assert(c[Z] >= 0);


      bool connected_1d = 
	(_p[c[X] + 1][c[Y]][c[Z]] ||
	 _p[c[X] - 1][c[Y]][c[Z]] ||
	 _not_empty[X][c[Y]][c[Z]] == false) &&
	
	(_p[c[X]][c[Y] + 1][c[Z]] ||
	 _p[c[X]][c[Y] - 1][c[Z]] ||
	 _not_empty[Y][c[X]][c[Z]] == false) &&
	
	(_p[c[X]][c[Y]][c[Z] + 1] ||
	 _p[c[X]][c[Y]][c[Z] - 1] ||
	 _not_empty[Z][c[X]][c[Y]] == false);
      
      bool connected_2d = 
	(_p[c[X] + 1][c[Y]    ][c[Z]] || _p[c[X] - 1][c[Y]    ][c[Z]] ||
	 _p[c[X]    ][c[Y] + 1][c[Z]] || _p[c[X]    ][c[Y] - 1][c[Z]] ||
	 _not_empty_2d_slice[Z][c[Z]] == false) &&
	
	(_p[c[X] + 1][c[Y]][c[Z]    ] || _p[c[X] - 1][c[Y]][c[Z]    ] ||
	 _p[c[X]    ][c[Y]][c[Z] + 1] || _p[c[X]    ][c[Y]][c[Z] - 1] ||
	 _not_empty_2d_slice[Y][c[Y]] == false) &&
	
	(_p[c[X]    ][c[Y] + 1][c[Z]] || _p[c[X]    ][c[Y] - 1][c[Z]] ||
	 _p[c[X]    ][c[Y]][c[Z] + 1] || _p[c[X]    ][c[Y]][c[Z] - 1] ||
	 _not_empty_2d_slice[X][c[X]] == false);
      

      /*if(debug_test_convexity(c)) {*/
      if ( connected_1d && 
	  (connected_2d || _is_strong_convex == false)) {
      
	_p[c[X]][c[Y]][c[Z]] = true;
	
	_not_empty[X][c[Y]][c[Z]] = true;
	_not_empty[Y][c[X]][c[Z]] = true;
	_not_empty[Z][c[X]][c[Y]] = true;

	_not_empty_2d_slice[X][c[X]] = true;
	_not_empty_2d_slice[Y][c[Y]] = true;
	_not_empty_2d_slice[Z][c[Z]] = true;

	_size++;

	_p_list.insert(c);

	for(int d = X; d < LAST_DIM; d++) {
	  _min[d] = min(_min[d], c[d]);
	  _max[d] = max(_max[d], c[d]);
	}
	
	
	return true;
      }
      
      return false;
    }
    
    /** 
     * Removes cell c from polycube.
     * There is an assumption here that c is always the last cell that was added.
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


      if (_p[c[X] + 1][c[Y]    ][c[Z]] == false && _p[c[X] - 1][c[Y]    ][c[Z]] == false &&
	  _p[c[X]    ][c[Y] + 1][c[Z]] == false && _p[c[X]    ][c[Y] - 1][c[Z]] == false) {
	_not_empty_2d_slice[Z][c[Z]] = false;
      }

      if (_p[c[X] + 1][c[Y]][c[Z]    ] == false && _p[c[X] - 1][c[Y]][c[Z]    ] == false &&
	  _p[c[X]    ][c[Y]][c[Z] + 1] == false && _p[c[X]    ][c[Y]][c[Z] - 1] == false) {
	_not_empty_2d_slice[Y][c[Y]] = false;
      }

      if (_p[c[X]    ][c[Y] + 1][c[Z]] == false && _p[c[X]    ][c[Y] - 1][c[Z]] == false &&
	  _p[c[X]    ][c[Y]][c[Z] + 1] == false && _p[c[X]    ][c[Y]][c[Z] - 1] == false) {
	_not_empty_2d_slice[X][c[X]] = false;
      }


    
      
      _size--;
      _p[c[X]][c[Y]][c[Z]] = false;
      
      _p_list.erase(c);
      


      /*
      for(int d = X; d < LAST_DIM; d++) {
	// Update _min
	if (c[d] == _min[d]) {
	  bool need_update = true;

	  for(set<Cell, CompareCellsLexi>::iterator iter = _p_list.begin(); iter != _p_list.end(); iter++) {
	    Cell c2 = *iter;

	    if (c2[d] == _min[d]) {
	      need_update = false;
	      break;
	    }
	  }
	  if(need_update) {
	    _min[d] ++;
	  }
	}

	// Update _max
	if (c[d] == _max[d]) {
	  bool need_update = true;
	  
	  for(set<Cell, CompareCellsLexi>::const_iterator iter = _p_list.begin(); iter != _p_list.end(); iter++) {
	    Cell c2 = *iter;
	    
	    if (c2[d] == _max[d]) {
	      need_update = false;
	      break;
	    }
	  } 
	  if(need_update) {
	    _max[d] --;
	  }
	}
	} */ 
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


    /**
     * Write the polycube into the file whose handle was provided during construction
     */
    void dump_to_file() const {
      if (_f->is_open() == false) {
	return;
      }

      vector<string> sorted_vertices;

      
      for(set<Cell, CompareCellsLexi>::const_iterator iter = _p_list.begin(); iter != _p_list.end(); iter++) {
	Cell c = *iter;
	stringstream strm;
	strm <<  "(" << c[X] - ORIGIN_COORD << "," << c[Y] - ORIGIN_COORD << "," << c[Z] - ORIGIN_COORD << ") ";
	
	(*_f) << strm.str();

      }

      (*_f) << endl; 
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


      for(set<Cell, CompareCellsLexi>::const_iterator iter = _p_list.begin(); iter != _p_list.end(); iter++) {
	Cell c = *iter;
	stringstream strm;
	strm <<  "(" << c[X] - ORIGIN_COORD << "," << c[Y] - ORIGIN_COORD << "," << c[Z] - ORIGIN_COORD << ") ";
	cout << strm.str();
      }

      cout << endl;

      cout << "min[X]: " << _min[X] << "max[X]: " << _max[X] << "min[Y]: " << _min[Y] << "max[Y]: " << _max[Y] << "min[Z]: " << _min[Z] << "max[Z]: " << _max[Z] << endl;
      
    }


    bool debug_test_convexity() {

      for(int ix = _min[X]; ix <= _max[X]; ix++) {
	for(int iy = _min[Y]; iy <= _max[Y]; iy++) {
	  bool started = false;
	  bool stopped = false;
	  for(int iz = _min[Z]; iz <= _max[Z]; iz++) {
	    if(_p[ix][iy][iz] && stopped)
	      return false;
	    else if(_p[ix][iy][iz])
	      started = true;
	    else if(!_p[ix][iy][iz] && started)
	      stopped = true;
	  }
	}
      }

      for(int ix = _min[X]; ix <= _max[X]; ix++) {
	for(int iz = _min[Z]; iz <= _max[Z]; iz++) {
	  bool started = false;
	  bool stopped = false;
	  for(int iy = _min[Y]; iy <= _max[Y]; iy++) {
	    if(_p[ix][iy][iz] && stopped)
	      return false;
	    else if(_p[ix][iy][iz])
	      started = true;
	    else if(!_p[ix][iy][iz] && started)
	      stopped = true;
	  }
	}
      }

      for(int iz = _min[Z]; iz <= _max[Z]; iz++) {
	for(int iy = _min[Y]; iy <= _max[Y]; iy++) {
	  bool started = false;
	  bool stopped = false;
	  for(int ix = _min[X]; ix <= _max[X]; ix++) {
	    if(_p[ix][iy][iz] && stopped)
	      return false;
	    else if(_p[ix][iy][iz])
	      started = true;
	    else if(!_p[ix][iy][iz] && started)
	      stopped = true;
	  }
	}
      }

      return true;

    }


    bool debug_test_convexity(Cell c) {
      assert(_p[c[X]][c[Y]][c[Z]] == false);
      
      _p[c[X]][c[Y]][c[Z]] = true;
      bool ret = debug_test_convexity();
      _p[c[X]][c[Y]][c[Z]] = false;

      return ret;
    } 
    

    

  };

  /**
   * Will add neighbours of the cell to the untried set u, which are not neighbours of
   * the polycube p or part of it. 
   *
   */
  void add_new_neig_to_untried(Cell& _c, UntriedSet* u, ConvexPolycube* p) {
    vector<Cell> candidates;


    for(int dim = 0; dim < LAST_DIM; dim++) {

      // -1, +1
      for (int offset = -1; offset <= 1; offset+=2) {
	Cell c2 = _c;


	// Notice that cells at the edge have neighbours beyond the edge. This is ok.
	c2[dim] += offset;
    
	if (c2[X] > 0) {
	  candidates.push_back(c2);
	} else if (c2[X] == 0) {
	  if (c2[Y] > 0) {
	    candidates.push_back(c2);
	  } else if (c2[Y] == 0) {
	    if (c2[Z] >= 0) {
	      candidates.push_back(c2);
	    } 
	  }
	}


      }
    }



    /* 26/07/15 experimenting - comparing with new version


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
      } */ 
    
    
    for (vector<Cell>::iterator iter=candidates.begin(); iter != candidates.end(); ++iter) {
      if (p->is_neigh(*iter, _c) == false && p->is_contained(*iter) == false) {
	u->push_back(*iter);
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
    
    list<Cell>::iterator iter = untried.begin();

    while(iter != untried.end()) {      
      
      if(curr_id >= hight_id) {
	return;
      }
          
      Cell c = *iter;

      // If the polycube 'p' with addition of cell 'c' becomes non convex, add will not add cell 'c' and return false.
      // The cell will not be removed from the untried list allowing any convex polycubes that are perfixed with
      // 'p' and contain 'c' to be reached through a different path. 
      if (p.add(c) == true) {

	iter = untried.erase(iter);

      } else {

	iter++;
	continue;

      }

      // Print progress
      static int x4 = 1;
      if(p.size() == 4) {
	cout << "Progress... "  << x4 << "/86" << endl;
       x4++;
      }

      // count only polyominoes that are in the search range
      if(p.size() >= n0 && curr_id >= low_id && curr_id < hight_id) {
	p.dump_to_file();
	(*results)[p.size()]++;
      }
      
      if(p.size() < n0 || (p.size() >= n0 && p.size() < n && curr_id >= low_id && curr_id < hight_id) ) {
	UntriedSet untried_next = UntriedSet(untried);
	
	// add neighbours of c to the untried set
	// will add only neghbours which are not negibours of p
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
  void line_convex_counter_3d(coord_t n,
			      coord_t n0,
			      count_t low_id,
			      count_t hight_id,
			      vector<count_t>* results,
			      std::ofstream* dump_file) {

    Cell origin(ORIGIN_COORD, ORIGIN_COORD, ORIGIN_COORD);

    ConvexPolycube p(origin, dump_file);


    UntriedSet untried;
    add_new_neig_to_untried(origin, &untried, &p); 
    
    count_t curr_id = 0;
  
    p.dump_to_file(); // dump (0,0,0)

    redelemeier_recursive(p, 
			  untried, 
			  n,
			  n0, 
			  curr_id, 
			  low_id, 
			  hight_id,
			  results); 
    p.remove(origin);
  }


  void full_convex_counter_3d(coord_t n,
			      coord_t n0,
			      count_t low_id,
			      count_t hight_id,
			      vector<count_t>* results, 
			      std::ofstream* dump_file) {

    Cell origin(ORIGIN_COORD, ORIGIN_COORD, ORIGIN_COORD);
    
    ConvexPolycube p(origin, dump_file, true);
    
    
    UntriedSet untried;
    add_new_neig_to_untried(origin, &untried, &p); 
    
    count_t curr_id = 0;
    
    p.dump_to_file(); // dump (0,0,0)
    
    redelemeier_recursive(p, 
			  untried, 
			  n,
			  n0, 
			  curr_id, 
			  low_id, 
			  hight_id,
			  results); 
    p.remove(origin);


  }



};
