#ifndef REDELMEIERALGORITHM_H
#define REDELMEIERALGORITHM_H
#include <time.h>
#include <list>
#include "redelmeiergraph.h"
#include "bigint.h"
using std::list;
/**
	@author Gadi Aleksandrowicz <gadial@tx.technion.ac.il>
*/
class RedelmeierAlgorithm{
public:
    RedelmeierAlgorithm(int dimensions, int _maxPolyominoSize, int _parallelStopSize=0,
			int _parallelTotalNumber=0,int _parallelCurrentNumber=0);
	
    ~RedelmeierAlgorithm();

	void run();
	void recurse(RedelmeierGraph::CellList untriedSet, int n);
	bool inParallelRange();
	bool parallelCheck(int n);
	void printResults();
private:
	RedelmeierGraph myGraph;
	BigInt* counter;
	BigInt* properCounter;
	int dimension;
	int maxPolyominoSize;
	int parallelStopSize;
	int parallelTotalNumber;
	int parallelCurrentNumber;
	int parallelPolyominoNumber;
	time_t startTime;
	time_t endTime;
};

#endif
