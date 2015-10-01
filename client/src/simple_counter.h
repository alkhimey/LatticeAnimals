#ifndef SIMPLE_COUNTER_H
#define SIMPLE_COUNTER_H

#include <string>
#include <sstream>


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

  void write_results_to_map(std::map< std::string, count_t > *results) { 

    for(count_t i = 0; i < _counter_table.size(); i++) {
      std::stringstream sstr;
      sstr << i;      
      (*results)[sstr.str()] = _counter_table[i];
    }
  }
};




#endif
