#include "../src/lattice_animal.h"
#include "catch.hpp"

TEST_CASE( "pow_nd", "[utility]" ) 
{
  CHECK( pow_nd(2, 3) == 8 );
  CHECK( pow_nd(1, 1) == 1 );
  CHECK( pow_nd(1, 3) == 1 );
  CHECK( pow_nd(3, 1) == 3 );
}


TEST_CASE( "basic lattice animal test", "[lattice aminal]")
{
  SECTION("d=2 n=5") {
    LatticeAnimal a = LatticeAnimal(2, 5) ;
    std::vector<coord_t> c00  {0, 0};
    std::vector<coord_t> c01  {0, 1};
    std::vector<coord_t> c02  {0, 2};
    std::vector<coord_t> c03  {0, 3};
    std::vector<coord_t> c04  {0, 4};

    REQUIRE(a.add(c00));

    CHECK_FALSE(a.is_neigh(c02));
    CHECK_FALSE(a.is_contained(c02));
    
    REQUIRE(a.add(c01));

    CHECK(a.is_neigh(c02));
    
    REQUIRE(a.add(c02));
    REQUIRE(a.add(c03));
    REQUIRE(a.add(c04));

    CHECK(a.is_contained(c02));
  }
}



TEST_CASE( "is contained test", "[lattice aminal]")
{
  SECTION("d=3 n=5") {
    LatticeAnimal a = LatticeAnimal(3, 5) ;
    std::vector<coord_t> c000  {0, 0, 0};
    std::vector<coord_t> c444  {4, 4, 4};
  
    REQUIRE_FALSE(a.is_contained(c000));
    REQUIRE(a.add(c000));
    CHECK(a.is_contained(c000));
    CHECK_FALSE(a.is_contained(c444));
    a.pop();
    CHECK_FALSE(a.is_contained(c000));

  }
}

TEST_CASE( "is neighbour test", "[lattice aminal]")
{
  SECTION("d=3 n=5") {
    LatticeAnimal a = LatticeAnimal(3, 5) ;
    std::vector<coord_t> c222  {2, 2, 2};
    std::vector<coord_t> c223  {2, 2, 3};
    std::vector<coord_t> c221  {2, 2, 1};
    std::vector<coord_t> c232  {2, 3, 2};
    std::vector<coord_t> c212  {2, 1, 2};
    std::vector<coord_t> c322  {3, 2, 2};
    std::vector<coord_t> c122  {1, 2, 2};

    std::vector<coord_t> c233  {2, 3, 3};

    CHECK_FALSE(a.is_neigh(c222));
    REQUIRE(a.add(c222));
    CHECK_FALSE(a.is_neigh(c222));
    CHECK(a.is_neigh(c223));
    CHECK(a.is_neigh(c221));
    CHECK(a.is_neigh(c232));
    CHECK(a.is_neigh(c212));
    CHECK(a.is_neigh(c322));
    CHECK(a.is_neigh(c122));

    CHECK_FALSE(a.is_neigh(c233));

    a.pop();

    CHECK_FALSE(a.is_neigh(c222));
    CHECK_FALSE(a.is_neigh(c223));
    CHECK_FALSE(a.is_neigh(c221));
    CHECK_FALSE(a.is_neigh(c232));
    CHECK_FALSE(a.is_neigh(c212));
    CHECK_FALSE(a.is_neigh(c322));
    CHECK_FALSE(a.is_neigh(c122));

    CHECK_FALSE(a.is_neigh(c233));
  }
}
