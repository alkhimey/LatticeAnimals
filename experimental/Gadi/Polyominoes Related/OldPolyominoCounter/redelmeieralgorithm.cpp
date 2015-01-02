#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#include "redelmeieralgorithm.h"
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::ostringstream;


RedelmeierAlgorithm::RedelmeierAlgorithm(int dimensions, int _maxPolyominoSize, int _parallelStopSize, 
			int _parallelTotalNumber,int _parallelCurrentNumber)
			:myGraph(dimensions,_maxPolyominoSize),dimension(dimensions),maxPolyominoSize(_maxPolyominoSize),
			parallelStopSize(_parallelStopSize),parallelTotalNumber(_parallelTotalNumber),
			parallelCurrentNumber(_parallelCurrentNumber)
{
	// here we have set the graph to contain the "empty polyomino"
	counter=new BigInt[maxPolyominoSize];
	properCounter=new BigInt[maxPolyominoSize];
	
	ifstream resultsFile("existingResults");
	string temp;
	ostringstream toCompare;
	toCompare<<"d="<<dimensions;
	do{
		resultsFile>>temp;
	} while (!resultsFile.eof() && temp!=toCompare.str());
	for (int i=1; i<parallelStopSize; i++){
		resultsFile>>temp>>temp;
	};
	resultsFile>>temp;
	resultsFile>>parallelPolyominoNumber;
	resultsFile.close();
}


RedelmeierAlgorithm::~RedelmeierAlgorithm()
{
}


void RedelmeierAlgorithm::run(){
	RedelmeierGraph::CellList untriedSet;
	RedelmeierGraph::CellInfo temp;
	temp.number=myGraph._0_Location;
	temp.dimension=0;
	startTime=time(NULL);
	untriedSet.push_front(temp); //"The untried set to contain only the origin"
	recurse(untriedSet,1);
	endTime=time(NULL);
	printResults();
}

void RedelmeierAlgorithm::printResults(){
	ostringstream outputFileName;
	outputFileName<<"results/TreeProperPolyominoCount-"<<dimension<<"d-"<<maxPolyominoSize<<"n-breakAt"<<parallelStopSize<<"-"<<parallelCurrentNumber<<"of"<<parallelTotalNumber<<".txt";
	ofstream outputFile(outputFileName.str().c_str());
	for (int i=0; i<maxPolyominoSize; i++){
		outputFile <<i+1<<": "<<properCounter[i]<<endl;
	}
	int totalTimeSeconds=endTime-startTime;
	outputFile<<"Time taken: "<<totalTimeSeconds<<" seconds, "
			<<(double)totalTimeSeconds/60 <<" minutes, "
			<<(double)totalTimeSeconds/3600 <<" hours, "
			<<(double)totalTimeSeconds/(3600*24) <<" days."<<endl;
	outputFile.close();

	ostringstream outputFileName2;
	outputFileName2<<"results/TreePolyominoCount-"<<dimension<<"d-"<<maxPolyominoSize<<"n-breakAt"<<parallelStopSize<<"-"<<parallelCurrentNumber<<"of"<<parallelTotalNumber<<".txt";
	ofstream outputFile2(outputFileName2.str().c_str());
	for (int i=0; i<maxPolyominoSize; i++){
		outputFile2 <<i+1<<": "<<counter[i]<<endl;
	}
	outputFile2<<"Time taken: "<<totalTimeSeconds<<" seconds, "
			<<(double)totalTimeSeconds/60 <<" minutes, "
			<<(double)totalTimeSeconds/3600 <<" hours, "
			<<(double)totalTimeSeconds/(3600*24) <<" days."<<endl;
	outputFile2.close();

}

void printUntriedSet(RedelmeierGraph::CellList untriedSet){
	RedelmeierGraph::CellList::iterator pos=untriedSet.begin();
	while (pos!=untriedSet.end()){
		cout << (*pos).number<<endl;
		pos++;
	}
}

bool RedelmeierAlgorithm::inParallelRange(){
	int chunkSize=(parallelPolyominoNumber/parallelTotalNumber)+1;
	int start=(parallelCurrentNumber-1)*chunkSize;
	int end=(parallelCurrentNumber)*chunkSize;
	if (counter[parallelStopSize-1]<start) return false;
	if (counter[parallelStopSize-1]>=end) return false;
	return true;
}

bool RedelmeierAlgorithm::parallelCheck(int n){
	return (!parallelStopSize || (n<parallelStopSize) || inParallelRange());
}
void RedelmeierAlgorithm::recurse(RedelmeierGraph::CellList untriedSet, int n){
	while (!untriedSet.empty()){ // "The following steps are repeated until the untried set is exhausted"
		RedelmeierGraph::CellList tempList;
		RedelmeierGraph::CellInfo current=untriedSet.front();
		untriedSet.pop_front(); // "1. Remove an arbitrary element from the untried set"
		tempList=myGraph.addCellToPolyomino(current); // "2. Place a cell at this point"
		counter[n-1]++; //"3. Count this new polyomino"
		myGraph.print_current_polyomino();
		if (myGraph.isProper()) properCounter[n-1]++;
		if (n<maxPolyominoSize && parallelCheck(n))	{//"4. If the size is less than maxPolyominoSize..."
			RedelmeierGraph::CellList untriedSetCopy=untriedSet;
			myGraph.purge_tree_ruiners(untriedSetCopy);
			untriedSetCopy.merge(tempList);
			recurse(untriedSetCopy,n+1);
		}
		myGraph.removeCellFromPolyomino(current); //"5. Remove newest cell"
	}
}

