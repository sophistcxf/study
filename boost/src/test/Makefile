CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

SIMPLE_TEST=simple_test

TARGETS=$(SIMPLE_TEST)

all: $(TARGETS)

$(SIMPLE_TEST):
	g++ -o $(SIMPLE_TEST) $(INCLUDE_PATH) $(CFLAGS) $(SIMPLE_TEST).cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

clean:
	rm -f $(TARGETS)
