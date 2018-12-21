CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

all: test_fixture simple_test

simple_test:
	g++ -o simple_test $(INCLUDE_PATH) $(CFLAGS) simple_test.cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

test_fixture:
	g++ -o test_fixture $(INCLUDE_PATH) $(CFLAGS) test_fixture.cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt
	

clean:
	rm -f test_fixture simple_test
