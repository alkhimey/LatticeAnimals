#include "../src/lattice_animal.h"
#include "catch.hpp"
#include <algorithm>

/**
 * This class is used to allow acces  of LatticeAnimal's inner
 * class fields.
 */
class LatticeAnimalBreakout : public LatticeAnimal {
public:
  LatticeAnimalBreakout(dim_t d, coord_t n) : LatticeAnimal(d, n) {};

  bool is_on_lattice(index_t idx) const {
    return _lattice[idx];
  }

  std::list< index_t > get_neighbours(index_t idx) const {
    return _neighbours[idx];
  }

  coord_t get_dim_size() const {
    return _dim_size;
  }

};


/**
 * Helper macro to verify that an element is inside a list.
 */

#define IN_LIST(lst, elem) std::find(std::begin(lst), std::end(lst), elem) != std::end(lst)





TEST_CASE( "pow_nd", "[utility]" ) 
{
  CHECK( pow_nd(2, 3) == 8 );
  CHECK( pow_nd(1, 1) == 1 );
  CHECK( pow_nd(1, 3) == 1 );
  CHECK( pow_nd(3, 1) == 3 );
  CHECK( pow_nd(12, 0) == 1);
}


TEST_CASE( "dim_size", "[inner], [precomputation]" )  {


  LatticeAnimalBreakout ab = LatticeAnimalBreakout(3,3);
  CHECK(ab.get_dim_size() == 5);

  ab = LatticeAnimalBreakout(1,2);
  CHECK(ab.get_dim_size() == 3);

}

TEST_CASE( "is_valid_cell", "[inner], [precomputation]" )  {

  std::vector< coord_t > c;


  SECTION("1d") {
    LatticeAnimal a = LatticeAnimal(1,4);

    CHECK(a.is_valid_cell({0}));
    CHECK(a.is_valid_cell({1}));
    CHECK_FALSE(a.is_valid_cell({-1}));
  }

  SECTION("2d") {
    LatticeAnimal a = LatticeAnimal(2,4);
     
    CHECK(a.is_valid_cell({0,0}));
    CHECK(a.is_valid_cell({1,1}));
    CHECK_FALSE(a.is_valid_cell({-1,-1}));
    CHECK_FALSE(a.is_valid_cell({-1,0}));
    CHECK_FALSE(a.is_valid_cell({0,-1}));
    CHECK(a.is_valid_cell({1,-1}));
    CHECK_FALSE(a.is_valid_cell({-1,1}));

  }

  SECTION("out of boundaries") {
    LatticeAnimal a = LatticeAnimal(2,4);

    CHECK_FALSE(a.is_valid_cell({0,4}));
    CHECK_FALSE(a.is_valid_cell({0,-4}));
    CHECK_FALSE(a.is_valid_cell({4,2}));
    CHECK_FALSE(a.is_valid_cell({-4,2}));

    // Notice: {-3,0} is inside lattice boundaries but still not valid.
    CHECK(a.is_valid_cell({0,3}));
    CHECK(a.is_valid_cell({3,-3}));



  }

}


TEST_CASE( "neighbours", "[inner], [precomputation]" )  {

  std::list< index_t > u;

  SECTION("basic, one dimensional") {
    LatticeAnimalBreakout ab = LatticeAnimalBreakout(1,2);
 
    u = ab.get_neighbours(1); // 0
    CAPTURE(ab.get_neighbours(0).size());
    CAPTURE(ab.get_neighbours(1).size());
    CAPTURE(ab.get_neighbours(2).size());
    
    REQUIRE(u.size() == 1);
    REQUIRE(u.front() == 2);

  }

  SECTION("previously failed d=3 n=3") {

    // Notice that X and Y are opposite than how it appeads in
    // literature.
    //
    // 20   21 | 22   23   24
    //         |
    //         |
    // 15   16 | 17   18   19
    //         |
    //
    // 10   11 | 12   13   14
    //         |
    //         -----
    //  5    6    7 |  8    9
    //              |
    //              |
    //  0    1    2 |  3    4
    //

    LatticeAnimalBreakout ab = LatticeAnimalBreakout(2,3);
    u = ab.get_neighbours(13);
    
    REQUIRE( IN_LIST(u, 12));
    REQUIRE( IN_LIST(u, 18));
    REQUIRE( IN_LIST(u, 14));
    REQUIRE( IN_LIST(u, 8));
    REQUIRE(u.size() == 4);
  }

  // TODO: Add more neighbour tests 3d

}

TEST_CASE( "calc_coordinates", "[inner]" ) 
{
  SECTION( "one dimensional" ) {
    
    LatticeAnimal a = LatticeAnimal(1, 2) ;
    
    std::vector< coord_t > c;

    c = a.calc_coordinates(0);
    REQUIRE( c.size() == 1 );
    CHECK( c[0] == -1 );


    // index  0  1  2  3  4  5  6  7  8  9  10  11  12 
    // c[0]  -6 -5 -4 -3 -2 -1  0  1  2  3   4   5   6
    a = LatticeAnimal(1, 7) ;

    c = a.calc_coordinates(12);
    REQUIRE( c.size() == 1 );
    CHECK( c[0] == 6 );
  }

  SECTION( "four dimensional" ) {
    
    LatticeAnimal a = LatticeAnimal(4, 10) ;

    std::vector< coord_t > c;

    c = a.calc_coordinates(0);
    REQUIRE( c.size() == 4 );
    CHECK( c[0] == -9 );
    CHECK( c[1] == -9 );
    CHECK( c[2] == -9 );
    CHECK( c[3] == -9 );

  }
}

TEST_CASE( "calc_index", "[inner]" ) {

  std::vector< coord_t > c;

  SECTION( "one dimensional" ) {
    
    LatticeAnimal a = LatticeAnimal(1, 2) ;

    c = {-1};
    CHECK(a.calc_index(c) == 0);
    c = {0};
    CHECK(a.calc_index(c) == 1);
    c = {1};
    CHECK(a.calc_index(c) == 2);

    a = LatticeAnimal(1, 15) ;
    
    c = {-14};
    CHECK( a.calc_index(c) == 0);

    c = {0};
    CHECK( a.calc_index(c) == 14);

    c = {14};
    CHECK( a.calc_index(c) == 28); // notice index begins at 0
  }


  SECTION( "two dimensional" ) {

    //                     c[1]
    //
    //        6, 7, 8       1
    //        3, 4, 5       0
    //        0, 1, 2      -1
    //
    // c[0]  -1  0  1
    //
    LatticeAnimal a = LatticeAnimal(2, 2) ;
    c = { -1, -1 };
    CHECK( a.calc_index(c) == 0 );
     
    c = { 0, -1 };
    CHECK( a.calc_index(c) == 1 );

    c = { 0, 1 };
    CHECK( a.calc_index(c) == 7 );
  }

  SECTION( "four dimensional" ) {
    
    LatticeAnimal a = LatticeAnimal(4, 10) ;
    
    std::vector< coord_t > c;

    c = { -9, -9, -9, -9 };
    CHECK( a.calc_index(c) == 0 );
  }
}


TEST_CASE( "index and coordinate calculation interleave", "[inner]") {

  for (dim_t d = 1; d < 4; d++) {
    for(coord_t n = 1; n < d+3; n++) {

      LatticeAnimal a = LatticeAnimal(d,n);
      
      for(index_t idx = 0; idx < pow_nd(2*n - 1, d); idx++) {
	
	CAPTURE( d );
	CAPTURE( n );
	CAPTURE( idx );
	
	CAPTURE(a.calc_coordinates(idx)[0]);

	REQUIRE(a.calc_index(a.calc_coordinates(idx)) == idx); 
      }
    }
  }	      
}





TEST_CASE( "size with pop", "[lattice aminal]")
{
  SECTION("basic") {
    LatticeAnimal a = LatticeAnimal(2, 5) ;
    REQUIRE(a.size() == 0);

    a.add(a.get_origin());

    REQUIRE(a.size() == 1);

    a.pop();

    REQUIRE(a.size() == 0);
  }

  SECTION("advanced") {
    LatticeAnimal a = LatticeAnimal(3, 30) ;
    REQUIRE(a.size() == 0);

    a.add(a.get_origin());
    a.add(0);
    a.add(1);
    a.add(2);
    REQUIRE(a.size() == 4);

    a.pop();
    a.pop();

    REQUIRE(a.size() == 2);

    a.add(3);
    a.add(4);
    a.add(5);
    a.add(6);

    REQUIRE(a.size() == 6);
  }
}

TEST_CASE( "is_full", "[lattice aminal]") {
  LatticeAnimal a = LatticeAnimal(2, 3) ;
  REQUIRE_FALSE(a.is_full()); 

  a.add(a.get_origin());

  REQUIRE_FALSE(a.is_full()); 
   
  a.add(0);

  REQUIRE_FALSE(a.is_full()); 

  a.add(1);

  REQUIRE(a.is_full());

  REQUIRE(a.size() == 3);
 
}


TEST_CASE( "add", "[lattice aminal]") {
  
  LatticeAnimalBreakout ab = LatticeAnimalBreakout(3, 5);

  for( index_t idx = 0; idx < pow_nd( 3 * 5 - 1 , 2 ); idx++) {
    REQUIRE_FALSE((ab.is_on_lattice(idx)));
  }

  ab.add(ab.get_origin());

  for( index_t idx = 1; idx < 5; idx++) {
    

    REQUIRE( ab.can_add(idx) );
    ab.add(idx);
    REQUIRE((ab.is_on_lattice(idx)));
    }
}

TEST_CASE( "can add", "[lattice aminal]") {

  LatticeAnimal a = LatticeAnimal(5, 8) ;
  
  a.add(a.get_origin());
  
  REQUIRE(a.can_add(0));
  a.add(0);
  REQUIRE(a.can_add(1));
  a.add(1);
  REQUIRE(a.can_add(2));
  a.add(2);
  REQUIRE(a.can_add(3));
  a.add(3);
  REQUIRE(a.can_add(4));
  a.add(4);
}




TEST_CASE( "get new untried", "[lattice aminal]")
{
  std::list< index_t >  untried;

  SECTION("basic origin only") {
    
    LatticeAnimal a = LatticeAnimal(2, 5) ;
    a.add(a.get_origin());
    
    untried = a.get_new_untried();
    CHECK(untried.size() == 2);

    a = LatticeAnimal(3, 5) ;
    a.add(a.get_origin());
    
    untried = a.get_new_untried();
    CHECK(untried.size() == 3);

  }


  SECTION("extensive") {
    
    //
    // 20   21   22   23   24
    // 15   16  (17)  18   19
    // 10   11  (12) (13)  14
    //  5    6    7    8    9
    //  0    1    2    3    4
    //

    LatticeAnimal a = LatticeAnimal(2, 3) ;
    a.add(a.get_origin()); // 12
    
    untried = a.get_new_untried();
    REQUIRE(untried.size() == 2);
    REQUIRE( IN_LIST(untried, 13) );
    REQUIRE( IN_LIST(untried, 17) );
	     
    a.add(13);
    untried = a.get_new_untried();
    REQUIRE(untried.size() == 3);
    REQUIRE( IN_LIST(untried, 18) );
    REQUIRE( IN_LIST(untried, 14) );
    REQUIRE( IN_LIST(untried, 8) );

    a.add(17);
    untried = a.get_new_untried();
    CHECK(untried.size() == 1);
    CHECK( IN_LIST(untried, 22) );


  }



  // TODO:
}


