#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <iostream>
#include <string>
#include <sys/stat.h> 
#include <sstream>
#include "redelmeieralgorithm.h"

using namespace std;
bool fileNotExistsFor(int d, int n, int breakPoint, int totalChunks, int i)
{
    ostringstream filename;
    filename<<"results/ProperPolyominoCount-"<<d<<"d-"<<n<<"n-breakAt"<<breakPoint<<"-"<<i<<"of"<<totalChunks<<".txt";
    struct stat stFileInfo;
    int intStat = stat(filename.str().c_str(),&stFileInfo);
    return (intStat!=0);
}
int main(int argc, char *argv[])
{
	if (argc!=6){
		cout <<"usage: PolyominoCounter [d] [n] [breakPoint] [totalChunks] [startingFrom]"<<endl;
		return EXIT_SUCCESS;
	}
    int d=atoi(argv[1]);
    int n=atoi(argv[2]);
    int breakPoint=atoi(argv[3]);
    int totalChunks=atoi(argv[4]);
    int startingFrom=atoi(argv[5]);
    for (int i=startingFrom; i<=totalChunks; i++)
    {
        if (fileNotExistsFor(d,n,breakPoint,totalChunks,i)){
            RedelmeierAlgorithm myAlg(d,n,breakPoint,totalChunks,i);
            myAlg.run();
        }
    }
  	return EXIT_SUCCESS;
}
