# here we modify the make file of whole test bench to make it only test SS problem

#  9th DIMACS Implementation Challenge: Shortest Paths
#  http://www.dis.uniroma1.it/~challenge9

#  Usage: > make

all:
	gen_gr
	gen_ss
	run_ss

gen_gr:
	cd ./scripts; $(MAKE) -i gen_gr

gen_ss:
	cd ./scripts; $(MAKE) -i gen_ss

run_ss:
	cd ./scripts; $(MAKE) -i run_ss

clean:
	cd ./gens;    $(MAKE) clean
	cd ./solvers; $(MAKE) clean
	cd ./utils;   $(MAKE) clean
	cd ./scripts; $(MAKE) clean