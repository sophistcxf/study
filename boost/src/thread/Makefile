CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

TEST_THREADPOOL=test_threadpool
TEST_THREAD=test_thread

TARGETS=$(TEST_THREADPOOL)

all: $(TARGETS)

$(TEST_THREADPOOL):
	g++ -o $(TEST_THREADPOOL) $(INCLUDE_PATH) $(CFLAGS) $(TEST_THREADPOOL).cpp $(LIB_PATH) -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

$(TEST_THREAD):
	g++ -o $(TEST_THREAD) $(INCLUDE_PATH) $(CFLAGS) $(TEST_THREAD).cpp $(LIB_PATH) -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

clean:
	rm -f $(TARGETS)
