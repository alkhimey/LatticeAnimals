#include <map>
#include <iostream>
#include "redelmeiergraph.h"
using std::cout;
using std::endl;

RedelmeierGraph::RedelmeierGraph(int _dimensions, int _polyominoSizeLimit):dimensions(_dimensions),polyominoSizeLimit(_polyominoSizeLimit),_0_Location(0),FreeSize(2*_polyominoSizeLimit-2)
{
	int i,b;
	for (i=1, b=1; i<dimensions; i++, _0_Location+=b*(polyominoSizeLimit-2),b*=(FreeSize)); /* computing _0_LOCATION */
	occupiedDimesnions=new int[dimensions];
	for (i=0; i<dimensions; i++)
	   occupiedDimesnions[i]=0;
}

RedelmeierGraph::~RedelmeierGraph()
{
	delete[] occupiedDimesnions;
}

RedelmeierGraph::CellType RedelmeierGraph::operator[](const BigInt& cellNum){
	
}

void RedelmeierGraph::checkAndAddCell(const BigInt& cellNum,CellList& listToAddTo, int dimension){ //marks cell as neighbor, and if it wasn't neighbor beofre, adds it to the list
	CellMap::iterator activeCell=cells.find(cellNum);
		if (((activeCell)==cells.end())) { // it is not the case that the cell is occupied or already a neighbor
			cells[cellNum].type=candidate;
			CellInfo temp;
			temp.number=cellNum;
			temp.dimension=dimension;
			listToAddTo.push_front(temp);
		}
	cells[cellNum].refCount++;
}
void RedelmeierGraph::checkNeighbors(const BigInt& cellNum){ // removes the cell if it's without neighbors
	CellMap::iterator activeCell=cells.find(cellNum);
	if (activeCell==cells.end())
		return;
	cells[cellNum].refCount--;
	if ((*activeCell).second.type!=candidate)
		return;
	if (cells[cellNum].refCount==0)
		cells.erase(activeCell);
}

RedelmeierGraph::CellList RedelmeierGraph::addCellToPolyomino(const CellInfo& cellInfo){
	BigInt cellNum=cellInfo.number;
	CellMap::iterator activeCell=cells.find(cellNum);
	if (activeCell!=cells.end() && ((*activeCell).second.type==occupied)) // cell already exists and is occupied
		throw CellNotEmpty;
	else{
		cells[cellNum].type=occupied;
		if (cellInfo.dimension!=0) occupiedDimesnions[cellInfo.dimension-1]++;
		CellList newList;
		int b=1;
		for (int j=1; j<=dimensions; j++){
			if (cellNum+b>=_0_Location)
				checkAndAddCell(cellNum+b,newList,j);
			if (cellNum-b>=_0_Location)
				checkAndAddCell(cellNum-b,newList,j);
			b*=(FreeSize);
		} 		
		return newList;
	}	
}
void RedelmeierGraph::removeCellFromPolyomino(const CellInfo& cellInfo){
	BigInt cellNum=cellInfo.number;
	CellMap::iterator activeCell=cells.find(cellNum);
	if (activeCell==cells.end() || ((*activeCell).second.type==candidate))  // cell is not occupied
		throw CellNotOccupied;
	cells[cellNum].type=candidate;
	if (cellInfo.dimension!=0) occupiedDimesnions[cellInfo.dimension-1]--;
	int b=1;
	for (int j=1; j<=dimensions; j++){
		if (cellNum+b>=_0_Location)
			checkNeighbors(cellNum+b);
		if (cellNum-b>=_0_Location)
			checkNeighbors(cellNum-b);
		b*=(FreeSize);
	} 
}
bool RedelmeierGraph::isProper(){
	bool result=true;
	for (int i=0; i<dimensions; i++){
		if (!occupiedDimesnions[i])
			result=false;
    }
	return result;
}
void RedelmeierGraph::print_current_polyomino(){
	debugPrint(1);
}

void RedelmeierGraph::purge_tree_ruiners(CellList& list_to_purge){
	for (CellList::iterator pos=list_to_purge.begin(); pos!=list_to_purge.end(); pos++){
		if (cells[(*pos).number].refCount>1)
			pos=list_to_purge.erase(pos);
	}
}

bool RedelmeierGraph::cellNotOccupied(const BigInt& cellNum){
	return (cells.find(cellNum)==cells.end() || cells[cellNum].type==candidate);
}

void RedelmeierGraph::debugPrint(int showOnlyOccupied){
	CellMap::iterator activeCell=cells.begin();
	for ( ;activeCell!=cells.end(); activeCell++){
		if ((*activeCell).second.type==occupied)
			cout << (*activeCell).first<< "-";
	}
	cout <<endl;
}

void RedelmeierGraph::debugPrintDimensions(){
	cout << "Dimensions occupied: (";
	for (int i=0; i<dimensions; i++)
		if (occupiedDimesnions[i]) cout << i+1<<",";
	cout << ")\n";
}

ostream& operator<<(ostream& o, RedelmeierGraph::Cell& cell_to_print){

}
