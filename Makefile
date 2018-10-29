all: btree types.h rule table cyk cnf chomsky_norm_form.h main
	g++ -std=c++11 *.o -o main

cfg_to_cnf: cnf unittest.cc
	g++ -std=c++11 chomsky_norm_form.o utils.o  unittest.cc -o cfg_to_cnf

main: main.cc
	g++ -c -std=c++11 main.cc

cyk: cyk.cc
	g++ -c -std=c++11 cyk.cc

btree: btree.cc
	g++ -c -std=c++11 btree.cc

rule: rule.cc
	g++ -c -std=c++11 rule.cc

table: table.cc
	g++ -c -std=c++11 table.cc
cnf: chomsky_norm_form.cc utils.cc
	g++ -c -std=c++11 chomsky_norm_form.cc utils.cc

clean:
	rm *.o
	rm main
	rm cfg_to_cnf
