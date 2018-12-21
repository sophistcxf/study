CXX = g++

CFLAGS=-std=c++98 -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

TARGETS=

test_if_then:
	g++ --std=c++0x ${INCLUDE_PATH} -o test_if_then test_if_then.cpp -L ../../lib

test_sort:
	g++ --std=c++0x ${INCLUDE_PATH} -o test_sort test_sort.cpp -L ../../lib

clean:
	rm -f test_if_then test_sort
