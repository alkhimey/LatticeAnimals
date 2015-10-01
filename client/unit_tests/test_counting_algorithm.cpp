/**
 *
 * These test cases are verified with OEIS. The name of each test is the OEIS sequence id.
 * 
 * These are only basic tests which are intended to execute quickly. For higher level of assutrance 
 * one need to run the desired algorithm manually.
 * Also, this file contains only test of non fractional executions.
 *
 */
#include "../src/lattice_animal.h"
#include "../src/wc_lattice_animal.h"
#include "../src/counting_algorithm.h"
#include "../src/simple_counter.h"
#include "catch.hpp"

TEST_CASE( "A001168 - Number of fixed polyominoes with n cells" , "[counting][nonfractional]" ) {

  coord_t n = 10;
  std::map<std::string, count_t> r;

  CountingAlgorithm algo = redelemeier_main<LatticeAnimal, SimpleCounter, 2>;
  algo(         n,  //coord_t n
       (coord_t)0,  //coord_t n0,
       (count_t)0,  //count_t low_id,
       (count_t)1,  //count_t  high_id,
       &r,          //std::vector<count_t>* results,
       NULL);       //std::ofstream* dump_file


  CHECK( r["0"]  == (count_t)0     );
  CHECK( r["1"]  == (count_t)1     );
  CHECK( r["2"]  == (count_t)2     );
  CHECK( r["3"]  == (count_t)6     );
  CHECK( r["4"]  == (count_t)19    );
  CHECK( r["5"]  == (count_t)63    );
  CHECK( r["6"]  == (count_t)216   );
  CHECK( r["7"]  == (count_t)760   );
  CHECK( r["8"]  == (count_t)2725  );
  CHECK( r["9"]  == (count_t)9910  );
  CHECK( r["10"] == (count_t)36446 );

}

