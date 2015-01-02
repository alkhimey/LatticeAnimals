#ifndef REDELMEIERGRAPH_H
#define REDELMEIERGRAPH_H
#include <list>
#include <map>
#include "bigint.h"
using std::list;
using std::map;
/**
	@author Gadi Aleksandrowicz <gadial@tx.technion.ac.il>
*/
//typedef list<BigInt> CellList;
class RedelmeierGraph{
public:
	typedef enum {empty,occupied=1, candidate} CellType;
	typedef enum {CellNotEmpty,CellNotOccupied} RedelmeierGraphException;
	struct CellInfo{
		BigInt number;
		int dimension;
		bool operator<(const CellInfo& rhs){return number<rhs.number;}
	};
	typedef list<CellInfo> CellList;
	struct Cell{
		CellType type;
		int refCount; // number of neigbors in the polyomino
		Cell(){type=empty; refCount=0;}
		int dimension;
	};
	typedef map<BigInt,Cell> CellMap;
	BigInt _0_Location;
	int FreeSize;

    	RedelmeierGraph(int _dimensions, int _polyominoSizeLimit);
    	~RedelmeierGraph();
	
	CellType operator[](const BigInt& cellNum);
	CellList addCellToPolyomino(const CellInfo& cellInfo); // returns - new neighbors;
	void removeCellFromPolyomino(const CellInfo& cellInfo);
	void debugPrint(int showOnlyOccupied=0);
	void debugPrintDimensions();
	
	bool isProper(); // returns true if the current polyomino is a proper d-dimensional polyomino ("uses" all d dimensions)
	void purge_tree_ruiners(CellList& list_to_purge);
	void print_current_polyomino();
	private:
	void checkAndAddCell(const BigInt& cellNum,CellList& listToAddTo, int dimension);
	void checkNeighbors(const BigInt& cellNum);
	bool cellNotOccupied(const BigInt& cellNum);
	CellMap cells;
	int dimensions;
	int polyominoSizeLimit;
	int* occupiedDimesnions;
};

ostream& operator<<(ostream& o, RedelmeierGraph::Cell& cell_to_print);
#endif
