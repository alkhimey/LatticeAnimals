
This software package is a collection of distributed algorithms implementations for enumerating lattice animals. 

Most of the implementations are based on Redelemeier algortihm.

![Free pentominoes](https://upload.wikimedia.org/wikipedia/commons/thumb/a/aa/All_18_Pentominoes.svg/440px-All_18_Pentominoes.svg.png)


The management of the enumeration effort is implemented using a django web server with sqlite database.

This package allows implementing counters that enumerate various classes of lattice animals as well as aggregating the results in various ways.

Lattice animal classes can have the following characteristics:

1. Fixed or free - whether or not two symmetrical lattice animals are considered distinct.
2. Dimension - polycubes and higher dimensions.
3. Common property - for example all the convex polyominoes.
4. Lattice - for example polyiamond (triangle lattice) or polyhex (hexagonal lattice).
5. Neighbor confections - for example polyknights


## Additional Documentation

* [Design](Design.md)


