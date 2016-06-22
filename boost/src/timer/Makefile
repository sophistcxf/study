CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

TEST_CPUTIMER=test_cputimer

TARGETS=$(TEST_CPUTIMER)

all: $(TARGETS)

$(TEST_CPUTIMER):
	g++ -o $(TEST_CPUTIMER) $(INCLUDE_PATH) $(CFLAGS) $(TEST_CPUTIMER).cpp $(LIB_PATH) -lboost_system -lboost_thread -lboost_filesystem -lboost_timer -lboost_chrono -lpthread -lrt

clean:
	rm -f $(TARGETS)
