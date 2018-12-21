CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

all: $(TARGETS)

test_unordered_map:
	g++ -o test_unordered_map $(INCLUDE_PATH) $(CFLAGS) test_unordered_map.cpp $(LIB_PATH) -lpthread -lrt

clean:
	rm -f test_unordered_map 
