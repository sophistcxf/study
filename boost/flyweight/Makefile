CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

all: $(TARGETS)

test_flyweight:
	g++ -o test_flyweight $(INCLUDE_PATH) $(CFLAGS) test_flyweight.cpp $(LIB_PATH) -lpthread -lrt

clean:
	rm -f $(TARGETS)
