all: prime_factors test_multiset

prime_factors: prime_factors.cc
	g++ -g -Wall -Werror -o $@ $< -std=c++11

test_multiset: test_multiset.cc multiset.h
	g++ -Wall -Werror -o $@ $< -std=c++11 -pthread -lgtest

clean:
	-rm -f prime_factors test_multiset
