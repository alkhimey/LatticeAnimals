/**
 *
 * These test cases are verified with OEIS. The name of each test is the OEIS sequence id and the description
 * as it appears in OIES.
 * 
 * These are only basic tests which are intended to execute quickly. For higher level of assurance 
 * one needs to run the desired algorithm manually with higher n value.
 *
 */
#include <string>

#include "../src/lattice_animal.h"
#include "../src/wc_lattice_animal.h"
#include "../src/counting_algorithm.h"
#include "../src/simple_counter.h"
#include "catch.hpp"

TEST_CASE( "A001168 - Number of fixed polyominoes with n cells." , "[counting][oeis]" ) {

  CountingAlgorithm algo = redelemeier_main<LatticeAnimal, SimpleCounter, 2>;

  std::map<std::string, count_t> calculated_results;

  std::map<std::string, count_t> oeis_results = {
    { "0",  0 },
    { "1",  1 },                               
    { "2",  2 },
    { "3",  6 },
    { "4",  19 },
    { "5",  63 },
    { "6",  216 },
    { "7",  760 },
    { "8",  2725 },
    { "9",  9910 },
    { "10", 36446 },
    { "11", 135268 },
    { "12", 505861 },
    { "13", 1903890 },
    { "14", 7204874 },
    { "15", 27394666 },
    { "16", 104592937 },
  };

  SECTION("No split, n = 10") {  
    algo(
	 (coord_t)10,          //coord_t n
	 (coord_t)0,           //coord_t n0,
	 (count_t)0,           //count_t low_id,
	 (count_t)1,           //count_t  high_id,
	 &calculated_results,  //std::vector<count_t>* results,
	 NULL                  //std::ofstream* dump_file
	 );
    
    for(auto const &result : calculated_results) {
      CHECK( result.second == oeis_results[result.first] );
    }
  }
  
  SECTION("Split to two, n_split = 5, n = 10") {

    std::map<std::string, count_t> calculated_results_pt_1;
    std::map<std::string, count_t> calculated_results_pt_2;

    const coord_t n_split = 5;
    
    count_t n_split_count = oeis_results[std::to_string(n_split)];

    algo(
	 (coord_t)10,          
	 (coord_t)n_split,           
	 (count_t)0,           
	 (count_t)n_split_count / 2,
	 &calculated_results_pt_1,  
	 NULL                  
	 );

    algo(
	 (coord_t)10,          
	 (coord_t)n_split,           
	 (count_t)n_split_count / 2,           
	 (count_t)n_split_count,
	 &calculated_results_pt_2,  
	 NULL                  
	 );

    REQUIRE( calculated_results_pt_1.size() == calculated_results_pt_2.size() );

    for(auto const &result_pt_1 : calculated_results_pt_1) {
      CAPTURE( result_pt_1.first );
      CAPTURE( result_pt_1.second );
      CAPTURE( calculated_results_pt_2[result_pt_1.first]);

      if (std::stoi(result_pt_1.first) < n_split) {
	CHECK( (result_pt_1.second + calculated_results_pt_2[result_pt_1.first])  == 0 );
      } else {
	CHECK( (result_pt_1.second + calculated_results_pt_2[result_pt_1.first])  == oeis_results[result_pt_1.first] );
      }
    }

    
  } 
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

TEST_CASE( "A067675 - Number of fixed convex polyominoes with n cells." , "[counting][oeis]" ) {

  CountingAlgorithm algo = redelemeier_main<WeaklyConvexLatticeAnimal, SimpleCounter, 2>;

  std::map<std::string, count_t> calculated_results;

  std::map<std::string, count_t> oeis_results = {
    { "0",  0 },
    { "1",  1 },                               
    { "2",  2 },
    { "3",  6 },
    { "4",  19 },
    { "5",  59 },
    { "6",  176 },
    { "7",  502 },
    { "8",  1374 },
    { "9",  3630 },
    { "10", 9312 },
    { "11", 23320 },
    { "12", 57279 },
    { "13", 138536 },
    { "14", 331032 },
    { "15", 793630 },
    { "16", 1841867 },
  };

  SECTION("No split, n = 12") {  
    algo(
	 (coord_t)10,          //coord_t n
	 (coord_t)0,           //coord_t n0,
	 (count_t)0,           //count_t low_id,
	 (count_t)1,           //count_t  high_id,
	 &calculated_results,  //std::vector<count_t>* results,
	 NULL                  //std::ofstream* dump_file
	 );
    
    for(auto const &result : calculated_results) {
      CHECK( result.second == oeis_results[result.first] );
    }
  }
  
  SECTION("Split to three, n_split = 7, n = 12") {

    std::map<std::string, count_t> calculated_results_pt_1;
    std::map<std::string, count_t> calculated_results_pt_2;
    std::map<std::string, count_t> calculated_results_pt_3;

    const coord_t n_split = 7;
    
    count_t n_split_count = oeis_results[std::to_string(n_split)];

    algo(
	 (coord_t)12,          
	 (coord_t)n_split,           
	 (count_t)0,           
	 (count_t)n_split_count / 3,
	 &calculated_results_pt_1,  
	 NULL                  
	 );

    algo(
	 (coord_t)12,          
	 (coord_t)n_split,           
	 (count_t)n_split_count / 3,           
	 (count_t)2*n_split_count / 3,
	 &calculated_results_pt_2,  
	 NULL                  
	 );

    algo(
	 (coord_t)12,          
	 (coord_t)n_split,           
	 (count_t)2*n_split_count / 3,           
	 (count_t)n_split_count,
	 &calculated_results_pt_3,  
	 NULL                  
	 );

    
    REQUIRE( calculated_results_pt_1.size() == calculated_results_pt_2.size() );
    REQUIRE( calculated_results_pt_1.size() == calculated_results_pt_3.size() );

    for(auto const &result_pt_1 : calculated_results_pt_1) {
      CAPTURE( result_pt_1.first );
      CAPTURE( result_pt_1.second );
      CAPTURE( calculated_results_pt_2[result_pt_1.first] );
      CAPTURE( calculated_results_pt_3[result_pt_1.first] );

      if (std::stoi(result_pt_1.first) < n_split) {
	CHECK( (result_pt_1.second + calculated_results_pt_2[result_pt_1.first] + calculated_results_pt_3[result_pt_1.first])  == 0 );
      } else {
	CHECK( (result_pt_1.second + calculated_results_pt_2[result_pt_1.first] + calculated_results_pt_3[result_pt_1.first])  == oeis_results[result_pt_1.first] );
      }
    }

    
  } 
}









