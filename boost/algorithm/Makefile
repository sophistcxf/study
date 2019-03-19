CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib


TEST_BM=test_boyer_moore

TARGETS=$(TEST_BM)

all: $(TARGETS)

$(TEST_BM):
	g++ -o $(TEST_BM) $(INCLUDE_PATH) $(CFLAGS) $(TEST_BM).cpp $(LIB_PATH) -lpthread -lrt

test_join:
	g++ -o test_join $(INCLUDE_PATH) $(CFLAGS) test_join.cpp $(LIB_PATH) -lpthread -lrt

clean:
	rm -f $(TARGETS)
