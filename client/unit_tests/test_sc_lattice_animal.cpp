#include "../src/sc_lattice_animal.h"
#include "catch.hpp"

/**
 * This class is used to allow acces  of LatticeAnimal's inner
 * class fields.
 */
class ScLatticeAnimalBreakout : public StronglyConvexLatticeAnimal {
public:
  ScLatticeAnimalBreakout(dim_t d, coord_t n) : StronglyConvexLatticeAnimal(d, n) {};

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

/*
TEST_CASE( "strongly convex constructor", "[strongly convex lattice animal][constructor]") {
  
  std::vector< std::pair< column_t, coord_t > > cols;

   SECTION("d=2, n=1") {
     ScLatticeAnimalBreakout wca = ScLatticeAnimalBreakout(2, 1);
     
     cols = wca.get_index_to_cols()[0];

     CHECK(cols[0].second == 0);
     CHECK(cols[1].second == 0);

     CHECK(cols[0].first == 0);
     CHECK(cols[1].first == 1);
   }


   SECTION("d=2, n=3") {
     ScLatticeAnimalBreakout wca = ScLatticeAnimalBreakout(2, 3);
     
     cols = wca.get_index_to_cols()[12];

     CHECK(cols[0].second == 0);
     CHECK(cols[1].second == 0);

     CHECK(cols[0].first == 2);
     CHECK(cols[1].first == 2+5);
   }

}


TEST_CASE( "strongly convex is_in_class", "[strongly convex lattice animal]") {
}
*/
