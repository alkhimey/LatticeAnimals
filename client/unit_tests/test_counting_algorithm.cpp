/**
 *
 * These test cases are verified with OEIS. The name of each test is the OEIS sequence id.
 * 
 * These are only basic tests which are intended to execute quickly. For higher level of assurance 
 * one needs to run the desired algorithm manually with higher n value.
 *
 * Also, this file contains only test of non fractional executions.
 *
 */
#include "../src/lattice_animal.h"
#include "../src/wc_lattice_animal.h"
#include "../src/counting_algorithm.h"
#include "../src/simple_counter.h"
#include "catch.hpp"

TEST_CASE( "A001168 - Number of fixed polyominoes with n cells." , "[counting][nonfractional]" ) {

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

TEST_CASE( "A001931 - Number of fixed 3-dimensional polycubes with n cells." , "[counting][nonfractional]" ) {

  coord_t n = 9;
  std::map<std::string, count_t> r;

  CountingAlgorithm algo = redelemeier_main<LatticeAnimal, SimpleCounter, 3>;
  algo(         n,  //coord_t n
       (coord_t)0,  //coord_t n0,
       (count_t)0,  //count_t low_id,
       (count_t)1,  //count_t  high_id,
       &r,          //std::vector<count_t>* results,
       NULL);       //std::ofstream* dump_file


  CHECK( r["0"]  == (count_t)0       );
  CHECK( r["1"]  == (count_t)1       );
  CHECK( r["2"]  == (count_t)3       );
  CHECK( r["3"]  == (count_t)15      );
  CHECK( r["4"]  == (count_t)86      );
  CHECK( r["5"]  == (count_t)534     );
  CHECK( r["6"]  == (count_t)3481    );
  CHECK( r["7"]  == (count_t)23502   );
  CHECK( r["8"]  == (count_t)162913  );
  CHECK( r["9"]  == (count_t)1152870 );

}


TEST_CASE( "A151830 - Number of fixed 4-dimensional polycubes with n cells." , "[counting][nonfractional]" ) {

  coord_t n = 8;
  std::map<std::string, count_t> r;

  CountingAlgorithm algo = redelemeier_main<LatticeAnimal, SimpleCounter, 4>;
  algo(         n,  //coord_t n
       (coord_t)0,  //coord_t n0,
       (count_t)0,  //count_t low_id,
       (count_t)1,  //count_t  high_id,
       &r,          //std::vector<count_t>* results,
       NULL);       //std::ofstream* dump_file


  CHECK( r["0"]  == (count_t)0        );
  CHECK( r["1"]  == (count_t)1        );
  CHECK( r["2"]  == (count_t)4        );
  CHECK( r["3"]  == (count_t)28       );
  CHECK( r["4"]  == (count_t)234      );
  CHECK( r["5"]  == (count_t)2162     );
  CHECK( r["6"]  == (count_t)21272    );
  CHECK( r["7"]  == (count_t)218740   );
  CHECK( r["8"]  == (count_t)2323730  );


}









