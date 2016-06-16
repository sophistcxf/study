CXX = g++

CFLAGS=-std=c++98 -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

TARGETS=

all: $(TARGETS)

$(TEST_LOG):
	g++ -o $(TEST_LOG) $(INCLUDE_PATH) $(CFLAGS) $(TEST_LOG).cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

$(TUTORIAL_FMT_STREAM):
	g++ -o $(TUTORIAL_FMT_STREAM) $(INCLUDE_PATH) $(CFLAGS) $(TUTORIAL_FMT_STREAM).cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

$(TEST_TRIVIAL_LOGGING):
	g++ -o $(TEST_TRIVIAL_LOGGING) $(INCLUDE_PATH) $(CFLAGS) $(TEST_TRIVIAL_LOGGING).cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lpthread -lrt

clean:
	rm -f $(TARGETS)
