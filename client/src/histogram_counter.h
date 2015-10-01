#ifndef HISTOGRAM_COUNTER_H
#define HISTOGRAM_COUNTER_H


class HistogramCounter  {

protected:
  
  std::vector< std::vector < count_t > > _counter_histo;
  
public:

 HistogramCounter(coord_t max_n, dim_t d) {
    _counter_histo = std::vector< std::vector <count_t > >(max_n + 1);

    for(coord_t i = 0; i < (coord_t)_counter_histo.size(); i++) {

      _counter_histo[i] = std::vector< count_t >( 2 + i * 2 * (d - 1) + 1  ,0);

    }
  }

  void increment(coord_t n, count_t c) {
    _counter_histo[n][c] += 1;
  }


  void write_results_to_map(std::map< std::string, count_t > *results) { 

    for(coord_t i = 0; i < (coord_t)_counter_histo.size(); i++) { 
      for(count_t j = 0; j < _counter_histo[i].size(); j++) {
	std::stringstream sstr;
	sstr << i << "," << j;      
	(*results)[sstr.str()] = _counter_histo[i][j];
      }
    }
  }
};




#endif
