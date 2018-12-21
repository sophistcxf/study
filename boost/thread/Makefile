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

test_mutex:
	g++ -o test_mutex $(INCLUDE_PATH) $(CFLAGS) test_mutex.cpp $(LIB_PATH) -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

test_tss:
	g++ -o test_tss $(INCLUDE_PATH) $(CFLAGS) test_tss.cpp $(LIB_PATH) -lboost_system -lboost_thread -lpthread -lrt

clean:
	rm -f $(TARGETS)
