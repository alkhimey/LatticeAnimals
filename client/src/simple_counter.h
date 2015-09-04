#ifndef SIMPLE_COUNTER_H
#define SIMPLE_COUNTER_H

#include "logging.h"

class SimpleCounter {

protected:
  
  std::vector<count_t> _counter_table;
  
public:

  SimpleCounter(coord_t max_n, dim_t d) {
    _counter_table = std::vector<count_t>(max_n + 1, 0);
  }



  void increment(coord_t n, count_t c) {
    _counter_table[n] += c;
  }


  void output_to_log(void) const {
    
    LOG4CXX_INFO(logger, "Printing results...");

    for(count_t i = 0; i < _counter_table.size(); i++) {
      LOG4CXX_INFO(logger, "Result for " << i << "\t" << "is " << _counter_table[i]); 
    }
  }

  
  /**
   * @pre Size of vector count_v is equal to max_n supplied on the counter instansiation (n+1)
   *
   */
  void output_to_vector(std::vector< count_t>* count_vect) const {
    
    assert(count_vect != NULL);
    assert((*count_vect).size() == _counter_table.size());
    
    for(count_t i = 0; i < _counter_table.size(); i++) {
      (*count_vect)[i] = _counter_table[i];
    }
  }

};




#endif
