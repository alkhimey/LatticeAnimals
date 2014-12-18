


copy again and again the untried set (the line
   CellSet untried_next = CellSet(untried);
just before the recursive call) while one can manage with only one copy.


computing repeatedly the neighbors of a given cell is redundant -
this computation can be done for all cell once at the beginning.
also, the fact that you do not consider the canonical cell makes you
count every polyomino (in the plane) n times, that is, you lose a factor
of n in the running time.

calculating the convexity incrementally.

not calculating the predicate when it is not needed.
