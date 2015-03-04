/**
 * @author  Artium Nihamkin <artium@nihamkin.com>
 * @date February 2015 
 *
 * @section DESCRIPTION
 * 
 * This class is used to keep track of the results of counting algorithms.
 * It has a separate counter for each polycube size.
 *
 */

#ifndef RESULTS_COUNTER_H
#define RESULTS_COUNTER_H

#include <vector>
#include <string>


class ResultsCounter {

public:

ResultsCounter();

/**
 * Reinitializes all counters
 */
void init();

/**
 * Increment counter at location n by 1
 */
void inc(PolySize i);

/**
 * Gets the counted value in a printable format
 */
std::string get_printable(PolySize i);


private:

std::vector<PolyCount> counters;



};



#endif
