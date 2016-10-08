CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

TEST_LOG=test_log
TUTORIAL_FMT_STREAM=tutorial_fmt_stream
TEST_TRIVIAL_LOGGING=test_trivial_logging
TEST_LOGGER=test_logger

TARGETS=$(TEST_LOG) $(TUTORIAL_FMT_STREAM) $(TEST_TRIVIAL_LOGGING) $(LOGGER)

adjacency_list_struct:
	g++ -o adjacency_list_struct $(INCLUDE_PATH) $(CFLAGS) adjacency_list_struct.cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

clean:
	rm -f $(TARGETS)
