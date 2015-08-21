#include "../src/wc_lattice_animal.h"
#include "catch.hpp"

/**
 * This class is used to allow acces  of LatticeAnimal's inner
 * class fields.
 */
class WcLatticeAnimalBreakout : public WeaklyConvexLatticeAnimal {
public:
  WcLatticeAnimalBreakout(dim_t d, coord_t n) : WeaklyConvexLatticeAnimal(d, n) {};

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


TEST_CASE( "constructor", "[weakly convex lattice animal]") {
  
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

}

TEST_CASE( "can_add", "[weakly convex lattice animal]") {

  
  SECTION("d=2, n=3") {
    WeaklyConvexLatticeAnimal wca = WeaklyConvexLatticeAnimal(2, 3);

    //
    // 20   21   22   23   24
    // 15   16  (17) (18) (19)
    // 10   11  (12)  13  (14)
    //  5    6    7    8    9
    //  0    1    2    3    4
    //

    wca.add(wca.get_origin()); 
    
    REQUIRE(wca.can_add(17));
    REQUIRE(wca.can_add(13));
    wca.add(17);

    REQUIRE(wca.can_add(16));
    REQUIRE(wca.can_add(22));
    REQUIRE(wca.can_add(18));
    REQUIRE(wca.can_add(13));
    wca.add(18);
  }
}


TEST_CASE( "can_add bug search", "[bug][weakly convex lattice animal]") {

  // 110  111  112  113  114  115  116  117  118  119  120
  //  99  100  101  102  103  104  105  106  107  108  109   
  //  88   89   90   91   92   93   94   95   96   97   98   
  //  77   78   79   80   81   82   83   84   85   86   87    
  //  66   67   68   69   70   71   72   73   74   75   76 
  //  55   56   57   58   59  (60)  61   62   63   64   65 
  //  44   45   46   47   48   49   50   51   52   53   54
  //  33   34   35   36   37   38   39   40   41   42   43   
  //  22   23   24   25   26   27   28   29   30   31   32   
  //  11   12   13   14   15   16   17   18   19   20   21   
  //   0    1    2    3    4    5    6    7    8    9   10   
  //


  WeaklyConvexLatticeAnimal wca = WeaklyConvexLatticeAnimal(2, 6);
  wca.add(wca.get_origin()); // 60

  SECTION("try 1") {
    REQUIRE(wca.can_add(71));
    wca.add(71);
    REQUIRE(wca.can_add(70)); 
    wca.add(70);
    REQUIRE(wca.can_add(69)); 
    wca.add(69);
    REQUIRE(wca.can_add(68));
    wca.add(68);
  
    REQUIRE(wca.can_add(67));
    REQUIRE(wca.can_add(79));
    REQUIRE(wca.can_add(72));
    REQUIRE(wca.can_add(61));

    wca.pop();
    wca.pop();
    wca.pop();
    wca.pop();
  
    REQUIRE(wca.can_add(71));
    REQUIRE(wca.can_add(61));
    wca.add(61);

    REQUIRE(wca.can_add(71));
    REQUIRE(wca.can_add(72));
    wca.add(72);
    REQUIRE(wca.can_add(71));
    REQUIRE(wca.can_add(83));
    wca.add(83);

    REQUIRE(wca.can_add(71));

    wca.pop();

    REQUIRE(wca.can_add(73));
    REQUIRE_FALSE(wca.can_add(74));
    wca.add(73);
    REQUIRE(wca.can_add(74));

    REQUIRE(wca.can_add(62));
    wca.add(62);
    REQUIRE(wca.can_add(63));

    // no luck so far...
  }

  SECTION("try 2, maybe add_origin is using the wrong add?") {
     wca.add(71);
     wca.add(72);
     wca.add(73);

     REQUIRE_FALSE(wca.can_add(62));

     // gotcha!
  }


}



TEST_CASE( "add and pop", "[weakly convex lattice animal]") {

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
}

