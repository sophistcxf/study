CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

TEST_LOG=test_log
TUTORIAL_FMT_STREAM=tutorial_fmt_stream
TEST_TRIVIAL_LOGGING=test_trivial_logging
TEST_LOGGER=test_logger

TARGETS=$(TEST_LOG) $(TUTORIAL_FMT_STREAM) $(TEST_TRIVIAL_LOGGING) $(LOGGER)

all: $(TARGETS)

$(TEST_LOG):
	g++ -o $(TEST_LOG) $(INCLUDE_PATH) $(CFLAGS) $(TEST_LOG).cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

$(TUTORIAL_FMT_STREAM):
	g++ -o $(TUTORIAL_FMT_STREAM) $(INCLUDE_PATH) $(CFLAGS) $(TUTORIAL_FMT_STREAM).cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

$(TEST_TRIVIAL_LOGGING):
	g++ -o $(TEST_TRIVIAL_LOGGING) $(INCLUDE_PATH) $(CFLAGS) $(TEST_TRIVIAL_LOGGING).cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lpthread -lrt

$(TEST_LOGGER):
	g++ -o $(TEST_LOGGER) $(INCLUDE_PATH) $(CFLAGS) $(TEST_LOGGER).cpp logger.cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

clean:
	rm -f $(TARGETS)
