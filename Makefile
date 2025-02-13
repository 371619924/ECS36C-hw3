all: test_multiset

test_multiset: test_multiset.cc multiset.h
  g++ -Wall -Werror -o $@ $< -std=c++11 -pthread -lgtest

clean: 
  -rm -f test_multiset



