We can't upload our files on PTA due to some unknown reason.
So we share it as a ANONYMOUS github repo.

https://anonymous.4open.science/r/ADS-Project-2/

Our data structure:
\solvers\mlb-dimacs\fiboheap_core.cc
\solvers\mlb-dimacs\binheap_core.cc

Reference:
https://www.cnblogs.com/skywang12345/p/3656005.html
https://www.cnblogs.com/skywang12345/p/3659069.html

1. Introduction

This is a project modified from:

DIMACS Challenge 9 Benchmark Platform
Version 1.1
October 30, 2006

And only ss problem is fully tested, implemented by our dijkstra algorithm using binomial queue and fibonaci heap.

If you are only interested in source code, we place them in 
./solvers/mlb-dimacs/ourStructure
Other part is the content of testbench, like timer, graph downloader and some utils, just neglect it.


2. How to run


To run it, simply run 
- make
in unix system (tested)
or
- mingw32-make
in windows system (not tested)

and then the result will in file
./results

, which show graph information and running time.