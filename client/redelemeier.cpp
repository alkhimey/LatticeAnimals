#include <stack>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>
#include <map>
#include <limits.h>
#include "redelemeier.h"

using namespace std;

enum Axis {
	X,Y,Z
};

class Cell3; // Forward definition
typedef Cell3 Cell;
typedef vector< Cell > CellStack;
typedef set< Cell > CellSet; 
typedef bool (*Predicate)(CellStack&) ;

class Cell3 {
public:
	int x,y,z;	

	Cell3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {};

	bool operator== (const Cell3 &other) const {
		return (x == other.x &&
            y == other.y &&
            z == other.z);
	}
 
	bool operator!= (const Cell3 &other) const {
		return !(*this == other);
	}

	bool operator< (const Cell3 &other) const {
		if (x == other.x && y == other.y && z < other.z)	return true; 
		if (x == other.x && y < other.y)		       	return true;
		if (x < other.x)				       	return true;
		return false;
	}

	CellSet getNeigh();
};

/* Run redelemeier algorithm. The result is returned through mycount. */ 
void runRedelemeier(unsigned int n,
		    unsigned int n0,
		    count_t lowId,
		    count_t hightId,
		    Predicate pred,
		    vector<count_t>* mycount, 
		    std::ofstream* dump_file);


/* The actual implementation of the algorithm. */
void recursiveCount(CellStack& p,
		    CellSet untried, 
		    unsigned int n, 
		    unsigned int n0,
		    count_t& curr_id,
		    count_t low_id,
		    count_t hight_id,
		    Predicate pred,
		    vector<count_t>* mycount, 
		    std::ofstream* dump_file);

void recursiveCountV2(CellStack& p,
		      CellSet untried, 
		      unsigned int n, 
		      unsigned int n0,
		      count_t& curr_id,
		      count_t low_id,
		      count_t hight_id,
		      Predicate pred,
		      vector<count_t>* mycount, 
		      std::ofstream* dump_file);

void iterativeCount(CellStack& p,
		    CellSet untried, 
		    unsigned int n, 
		    unsigned int n0,
		    count_t& curr_id,
		    count_t low_id,
		    count_t hight_id,
		    Predicate pred,
		    vector<count_t>* mycount, 
		    std::ofstream* dump_file);


/* Predicate that matches 3d fully-convex polycubes. */
bool predConvex2_3d(CellStack& p);

/* Predicate that matched 3d line-convex polycubes. */
bool predConvex1_3d(CellStack& p);

/* Predicate that matches all polycubes */
bool predAll(CellStack& p);

/* Dumps cell stack to file */
void dump_cell_stack(CellStack& p, std::ofstream* dump_file);


CellSet Cell3::getNeigh() {
	CellSet s;
	Cell3 c(x,y,z);

	c.x++; s.insert(c);
	c.x--; 
	if(!(z==0 && ((x==0 && y >= 0) || (x==1 && y < 0)))) {
		c.x--; s.insert(c);
		c.x++;	
	}

	c.y++; s.insert(c);
	c.y--;	
	if(!(x==0 && y==0 && z==0)) {
		c.y--;	s.insert(c);
		c.y++;	
	}

	c.z++; s.insert(c);
	c.z--;	
	if(z > 1 || (z == 1 && x>=0 && y>=0) || (z == 1 && x>=1 && y < 0)) {
		c.z--;	s.insert(c);
	}

	return s;
}


/* Returns true if the cell c is contained in p */
bool isContained(Cell c, CellStack& p) {
	for(CellStack::iterator iter = p.begin(); iter != p.end(); iter++)
		if(*iter == c)
			return true;
	return false;
}

/* New untried is a cell of which none of the neighboors is inside p except the trivial case of last added cell to p  */
bool isNewUntried(Cell c, CellStack& p) {
	CellSet s = c.getNeigh();
	s.insert(c);
	for(CellSet::iterator iter = s.begin(); iter != s.end(); iter++)
		if(isContained(*iter, p) && *iter != p.back())
			return false;
	return true;
}

void runRedelemeier(unsigned int n, unsigned int n0, count_t lowId, count_t hightId, Predicate pred, vector<count_t>* mycount, std::ofstream* dump_file) {
	CellStack p;
	CellSet untried;
	untried.insert(Cell(0,0,0));
	count_t currId = 0;
	
	recursiveCountV2(p, untried, n, n0, currId, lowId, hightId, pred, mycount, dump_file); 
	//iterativeCount(p, untried, n, n0, currId, lowId, hightId, pred, mycount);
}

void recursiveCount(CellStack& p, CellSet untried, unsigned int n,
					unsigned int n0, count_t &curr_id, count_t low_id, count_t hight_id,
		    Predicate pred, vector<count_t>* mycount, std::ofstream* dump_file) 
{	
   	while(!untried.empty()) {
	
		if(curr_id >= hight_id)
			return;

		p.push_back(*(untried.begin()));
		untried.erase(untried.begin());

		// Count only polyominoes that match the predicate and are in the search range //
		if(pred(p) && p.size() >= n0 && curr_id >= low_id && curr_id < hight_id) {
			(*mycount)[p.size()]++;
			dump_cell_stack(p, dump_file);
		}

		if(p.size() < n0 || (p.size() >= n0 && p.size() < n && curr_id >= low_id && curr_id < hight_id) ) {
			CellSet untried_next = CellSet(untried);
			CellSet s = p.back().getNeigh();
			for(CellSet::iterator iter = s.begin(); iter != s.end(); iter++) 
				if(isNewUntried(*iter, p))
					untried_next.insert(*iter);		
			recursiveCount(p, untried_next, n, n0, curr_id, low_id, hight_id, pred, mycount, dump_file);
		}
		
		if(p.size() == n0) 
			curr_id++;
		
		p.pop_back();
	}
}



void recursiveCountV2(CellStack& p, CellSet untried, unsigned int n,
					unsigned int n0, count_t &curr_id, count_t low_id, count_t hight_id,
		      Predicate pred, vector<count_t>* mycount, std::ofstream* dump_file) 
{	
   	while(!untried.empty()) {
	
		if(curr_id >= hight_id)
			return;

		p.push_back(*(untried.begin()));
		untried.erase(untried.begin());

		// Count only polyominoes that match the predicate and are in the search range //
		if(pred(p) && p.size() >= n0 && curr_id >= low_id && curr_id < hight_id) {
			(*mycount)[p.size()]++;
			dump_cell_stack(p, dump_file);
		}

		if(p.size() < n0 || (p.size() >= n0 && p.size() < n && curr_id >= low_id && curr_id < hight_id) ) {
			CellSet untried_next = CellSet(untried);
			CellSet s = p.back().getNeigh();
			for(CellSet::iterator iter = s.begin(); iter != s.end(); iter++) 
				if(isNewUntried(*iter, p))
					untried_next.insert(*iter);		
			recursiveCount(p, untried_next, n, n0, curr_id, low_id, hight_id, pred, mycount, dump_file);
		}
		
		if(p.size() == n0) 
			curr_id++;
		
		p.pop_back();
	}
}

/* This predicate matches all polyominoes */
bool predAll(CellStack& p) {
	return true;
}


/* Matches only convex polyominoes according to the first definition.
   works only with 3d polyominoes 
*/  
bool predConvex1_3d(CellStack& p) {

	int minx = INT_MAX, miny = INT_MAX, minz = INT_MAX;
	int maxx = INT_MIN, maxy = INT_MIN, maxz = INT_MIN;

	for(CellStack::iterator iter = p.begin(); iter < p.end(); iter++) { 
		minx = min(minx, (*iter).x);
		miny = min(miny, (*iter).y);
		minz = min(minz, (*iter).z);
		maxx = max(maxx, (*iter).x);
		maxy = max(maxy, (*iter).y);
		maxz = max(maxz, (*iter).z);
	}

	int sizex = maxx - minx + 1;
	int sizey = maxy - miny + 1;
	int sizez = maxz - minz + 1;

	// Create 3d matrix and fill it //
	vector< vector < vector < bool > > > m(sizex, 
		vector < vector < bool > >(sizey, 
			vector< bool >(sizez, false))
	);
	for(CellStack::iterator iter = p.begin(); iter < p.end(); iter++) {
		m[(*iter).x - minx][(*iter).y - miny][(*iter).z - minz] = true;
	}

	// Check if polyomino in the matrix is convex. Unfortunate code duplication here //
	for(int ix = 0; ix < sizex; ix++) {
		for(int iy = 0; iy < sizey; iy++) {
			bool started = false;
			bool stopped = false;
			for(int iz = 0; iz < sizez; iz++) {
				if(m[ix][iy][iz] && stopped)
					return false;
				else if(m[ix][iy][iz])
					started = true;
				else if(!m[ix][iy][iz] && started)
					stopped = true;
			}
		}
	}

	for(int ix = 0; ix < sizex; ix++) {
		for(int iz = 0; iz < sizez; iz++) {
			bool started = false;
			bool stopped = false;
			for(int iy = 0; iy < sizey; iy++) {
				if(m[ix][iy][iz] && stopped)
					return false;
				else if(m[ix][iy][iz])
					started = true;
				else if(!m[ix][iy][iz] && started)
					stopped = true;
			}
		}
	}

	for(int iz = 0; iz < sizez; iz++) {
		for(int iy = 0; iy < sizey; iy++) {
			bool started = false;
			bool stopped = false;
			for(int ix = 0; ix < sizex; ix++) {
				if(m[ix][iy][iz] && stopped)
					return false;
				else if(m[ix][iy][iz])
					started = true;
				else if(!m[ix][iy][iz] && started)
					stopped = true;
			}
		}
	}

	return true;
}

/* Matches vectors that represents a valid 2d polyomino that is also line convex.
   Helper for predConvex2_3d*/ 
bool isConvexPolyomino_2d(vector<pair<int, int> > p) {
	int minx = INT_MAX, miny = INT_MAX;
	int maxx = INT_MIN, maxy = INT_MIN;

	for(vector<pair<int, int> >::iterator iter = p.begin(); iter < p.end(); iter++) { 
		minx = min(minx, (*iter).first);
		miny = min(miny, (*iter).second);
		maxx = max(maxx, (*iter).first);
		maxy = max(maxy, (*iter).second);
	}

	int sizex = maxx - minx + 1;
	int sizey = maxy - miny + 1;
	
	// Create 2d matrix and fill it. also create a graph. //
	vector< vector < bool > >  m(sizex,  vector< bool >(sizey, false));
	map<pair<int, int>, vector< pair<int, int> > > graph;
	
	for(vector<pair<int, int> >::iterator iter = p.begin(); iter < p.end(); iter++) {
		m[(*iter).first - minx][(*iter).second - miny] = true;

		pair<int, int> xypoint((*iter).first, (*iter).second);
		graph[xypoint] = vector< pair<int, int> >();

		set< pair<int, int> > s;
		s.insert(pair<int, int>(xypoint.first, xypoint.second + 1));
		s.insert(pair<int, int>(xypoint.first, xypoint.second - 1));
		s.insert(pair<int, int>(xypoint.first + 1, xypoint.second));
		s.insert(pair<int, int>(xypoint.first - 1, xypoint.second));

		for(set< pair<int, int> >::iterator iter = s.begin(); iter != s.end(); iter++) {
			if(graph.find(*iter) != graph.end()) {
				graph[xypoint].push_back(*iter);
				graph[*iter].push_back(xypoint);
			}
		}
	}
	
	// Check if polyomino in the matrix is convex. Unfortunate code duplication here //
	for(int ix = 0; ix < sizex; ix++) {
		bool started = false;
		bool stopped = false;
		for(int iy = 0; iy < sizey; iy++) {
			if(m[ix][iy] && stopped)
				return false;
			else if(m[ix][iy])
				started = true;
			else if(!m[ix][iy] && started)					
				stopped = true;
		}
	}

	for(int iy = 0; iy < sizey; iy++) {
		bool started = false;
		bool stopped = false;
		for(int ix = 0; ix < sizex; ix++) {
			if(m[ix][iy] && stopped)
				return false;
			else if(m[ix][iy])
				started = true;
			else if(!m[ix][iy] && started)					
				stopped = true;
		}
	}

	// Check if the polyomino is connected //
	vector<pair<int, int> > discovered;
	set<pair<int, int> > visited;

	discovered.push_back(p.back());
	visited.insert(p.back());

	while(!discovered.empty()) {
		pair<int, int> c = discovered.back();
		discovered.pop_back();
		
		for(vector< pair<int, int> >::iterator iter = graph[c].begin(); iter < graph[c].end(); iter++) {
			if(visited.find(*iter) == visited.end()) {
				discovered.push_back(*iter);
				visited.insert(*iter);
			}
		}
	}

	return p.size() == visited.size();
}




/* Matches only convex polycube according to the second definition.
   works only with 3d polycubes.
   A polycube is convex if every cross-section (parallel to xy, xz or yz is convex 2d polyomino.
*/ 
bool predConvex2_3d(CellStack& p) {
	// Generate all cross sections //
	map<pair<Axis, int>, vector<pair<int, int > > > v; // All cross sections in vector format
	
	for(CellStack::iterator iter = p.begin(); iter < p.end(); iter++) {
		Cell c = *iter;
		v[pair<Axis, int>(X, c.x)].push_back(pair<int, int>(c.y,c.z));
		v[pair<Axis, int>(Y, c.y)].push_back(pair<int, int>(c.x,c.z));
		v[pair<Axis, int>(Z, c.z)].push_back(pair<int, int>(c.x,c.y));
	}

	// For each cross section check if it contains a convex polyomino //
	map<pair<Axis, int>, vector<pair<int, int > > >::iterator iter;
	for(iter = v.begin(); iter != v.end(); iter++) {
		if (!isConvexPolyomino_2d((*iter).second))
			return false;		
	}

	return true;
}

/**
 * Dump the polycube into a file. Used for debugging purposes.
 */ 
void dump_cell_stack(CellStack& p, std::ofstream* dump_file) {

  if (dump_file->is_open() == false) {
    return;
  }

  
  for(CellStack::iterator iter = p.begin(); iter < p.end(); iter++) {
  
    Cell c = *iter;
    (*dump_file) << "(" << c.x << "," << c.y << "," << c.z << ") ";
  }
  
  (*dump_file) << endl;


}



void redelemeier_recursive_3d(unsigned int n,
			      unsigned int n0,
			      count_t lowId,
			      count_t hightId,
			      vector<count_t>* results,
			      std::ofstream* dump_file) {
  

  runRedelemeier(n, n0, lowId, hightId, predAll, results, dump_file);


}


void redelemeier_3d_line_convex(unsigned int n,
				unsigned int n0,
				count_t lowId,
				count_t hightId,
				vector<count_t>* results, 
				std::ofstream* dump_file) {
  runRedelemeier(n, n0, lowId, hightId, predConvex1_3d, results, dump_file);

}

void redelemeier_3d_full_convex(unsigned int n,
				unsigned int n0,
				count_t lowId,
				count_t hightId,
				vector<count_t>* results,
				std::ofstream* dump_file) {
  runRedelemeier(n, n0, lowId, hightId, predConvex2_3d, results, dump_file);

}


