#ifndef HISTOGRAM_COUNTER_H
#define HISTOGRAM_COUNTER_H

#include <iostream>
#include "logging.h"
#include "simple_counter.h"

class HistogramCounter : public SimpleCounter {

protected:
  
  std::vector< std::vector < count_t > > _counter_histo;
  
public:

 HistogramCounter(coord_t max_n, dim_t d) : SimpleCounter(max_n, d) {
    _counter_histo = std::vector< std::vector <count_t > >(max_n + 1);

    for(coord_t i = 0; i < (coord_t)_counter_histo.size(); i++) {

      _counter_histo[i] = std::vector< count_t >( 2 + i * 2 * (d - 1) + 1  ,0);

    }
  }

  void inline increment(coord_t n, count_t c) {
    _counter_histo[n][c] += 1;
  }


  void output_to_log(void) const {
    
    LOG4CXX_INFO(logger, "Printing results...");
    LOG4CXX_INFO(logger, "n\tcount\t#polyominoes");

    
    for(coord_t i = 0; i < (coord_t) _counter_histo.size(); i++) {

      count_t j = 0;
      
      // Ignore leading zeros
      while (j < _counter_histo[i].size() &&
             _counter_histo[i][j] == 0) {
    	 j++;
    	}

      for(; j < _counter_histo[i].size(); j++) {
	       LOG4CXX_INFO(logger, i << "\t" << j  << "\t" <<
                             _counter_histo[i][j]);
      }
    }
  }


};




#endif
