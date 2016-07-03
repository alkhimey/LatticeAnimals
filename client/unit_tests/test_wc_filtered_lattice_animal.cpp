#include "../src/wc_filtered_lattice_animal.h"
#include "catch.hpp"

/**
 * This class is used to allow acces  of LatticeAnimal's inner
 * class fields.
 */
class WcFilteredLatticeAnimalBreakout : public WeaklyConvexFilteredLatticeAnimal {
public:
  WcFilteredLatticeAnimalBreakout(dim_t d, coord_t n) : WeaklyConvexFilteredLatticeAnimal(d, n) {};

  bool is_on_lattice(index_t idx) const {
    return _lattice[idx];
  }

  std::list< index_t > get_neighbours(index_t idx) const {
    return _neighbours[idx];
  }

  coord_t get_dim_size() const {
    return _dim_size;
  }

  std::vector< ColumnDataType > get_columns() const {
    return _columns;
  }

  std::vector < std::vector< std::pair< column_t, coord_t > > > get_index_to_cols() const {
    return _index_to_cols;
  }

};


TEST_CASE( "weakly convex filtered constructor", "[weakly convex lattice animal][constructor]") {
  //TODO
/*
  std::vector< std::pair< column_t, coord_t > > cols;

   SECTION("d=2, n=1") {
     WcLatticeAnimalBreakout wca = WcLatticeAnimalBreakout(2, 1);
     
     cols = wca.get_index_to_cols()[0];

     CHECK(cols[0].second == 0);
     CHECK(cols[1].second == 0);

     CHECK(cols[0].first == 0);
     CHECK(cols[1].first == 1);
   }


   SECTION("d=2, n=3") {
     WcLatticeAnimalBreakout wca = WcLatticeAnimalBreakout(2, 3);
     
     cols = wca.get_index_to_cols()[12];

     CHECK(cols[0].second == 0);
     CHECK(cols[1].second == 0);

     CHECK(cols[0].first == 2);
     CHECK(cols[1].first == 2+5);
   }
*/
}




TEST_CASE( "add and pop weakly convex filtered", "[weakly convex lattice animal]") {

// TODO

/*
  SECTION("d=2, n=6") {
 
    WeaklyConvexLatticeAnimal wca = WeaklyConvexLatticeAnimal(2, 6);
    wca.add(wca.get_origin()); // 60

    SECTION("Straight line") {
      REQUIRE(wca.can_add(71));
      wca.add(71);
      
      REQUIRE(wca.can_add(82));
      wca.add(82);
      
      REQUIRE(wca.can_add(93));
      wca.add(93);
      
      REQUIRE(wca.can_add(104));
      wca.add(104);
      
      REQUIRE(wca.can_add(115));
      wca.add(115);
      
      wca.pop();
      wca.pop();
      wca.pop();
      wca.pop();
      wca.pop();
      
      REQUIRE(wca.can_add(71));
      REQUIRE(wca.can_add(61));
    }

    SECTION("Hooks") {

      REQUIRE(wca.can_add(71));
      wca.add(71);

      REQUIRE(wca.can_add(82));
      wca.add(82);

      REQUIRE(wca.can_add(83));
      wca.add(83);

      REQUIRE(wca.can_add(84));
      wca.add(84);

      REQUIRE_FALSE(wca.can_add(73));

      wca.pop();
      wca.pop();
      wca.pop();
      wca.pop();

      REQUIRE(wca.can_add(61));
      wca.add(61);

      REQUIRE(wca.can_add(62));
      wca.add(62);
      
      REQUIRE(wca.can_add(63));
      wca.add(63);
      
      REQUIRE(wca.can_add(73));
      wca.add(73);

      REQUIRE(wca.can_add(84));
      wca.add(84);

      REQUIRE_FALSE(wca.can_add(83));

    }

  }
*/
}

