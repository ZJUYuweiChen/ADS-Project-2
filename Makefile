# ============================================================================
#  Makefile
# ============================================================================
# here we modify the make file of whole test bench to make it only test SS problem

#  9th DIMACS Implementation Challenge: Shortest Paths
#  http://www.dis.uniroma1.it/~challenge9

#  Usage: > make         (Compiles generators, solvers, etc.)


all:
	gen_gr
	gen_ss
	run_ss

code:
	cd ./gens;    $(MAKE)
	cd ./solvers; $(MAKE)
	cd ./utils;   $(MAKE)

gen:
	cd ./scripts; $(MAKE) -i

run:
	cd ./scripts; $(MAKE) -i run

gen_gr:
	cd ./scripts; $(MAKE) -i gen_gr

gen_ss:
	cd ./scripts; $(MAKE) -i gen_ss

run_ss:
	cd ./scripts; $(MAKE) -i run_ss

gen_p2p:
	cd ./scripts; $(MAKE) -i gen_p2p

run_p2p:
	cd ./scripts; $(MAKE) -i run_p2p

clean:
	cd ./gens;    $(MAKE) clean
	cd ./solvers; $(MAKE) clean
	cd ./utils;   $(MAKE) clean
	cd ./scripts; $(MAKE) clean



# here are other part of this test bench.

#  Usage: > make gen     (Generates in ./inputs all core input families)
#  Usage: > make run     (Generates in ./results performace reports of benchmark  
#                         solvers on all core input families)

#  Usage: > make gen_gr  (Generates in ./inputs all gr input instances)

#  Usage: > make gen_ss  (Generates in ./inputs all ss input instances)
#  Usage: > make run_ss  (Generates in ./results performace reports of benchmark  
#                         solvers on ss core instances)

#  Usage: > make gen_p2p (Generates in ./inputs all p2p input instances)
#  Usage: > make run_p2p (Generates in ./results performace reports of benchmark  
#                         solvers on p2p core instances)

#  Usage: > make clean   (Cleans up the package)
