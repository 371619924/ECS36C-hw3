all: prime_factors test_multiset

prime_factors: prime_factors.cc
	g++ -Wall -Werror -std=c++11 -o prime_factors prime_factors.cc

test_multiset: test_multiset.cc multiset.h
	g++ -Wall -Werror -std=c++11 -o test_multiset test_multiset.cc -pthread -lgtest

clean:
	-rm -f test_multiset prime_factors
