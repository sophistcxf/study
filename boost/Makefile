CXX = g++

CFLAGS=-std=c++98 -g 
INCLUDE_PATH=-I ../include

TEST_TOKENIZER=test_tokenizer
TEST_LOG=test_log
TUTORIAL_FMT_STREAM=tutorial_fmt_stream
TEST_TRIVIAL_LOGGING=test_trivial_logging

TARGETS=$(TEST_TOKENIZER) $(TEST_LOG) $(TUTORIAL_FMT_STREAM) $(TEST_TRIVIAL_LOGGING)

all: $(TARGETS)

$(TEST_TOKENIZER):
	g++ -o $(TEST_TOKENIZER) $(INCLUDE_PATH) $(CFLAGS) $(TEST_TOKENIZER).cpp

$(TEST_LOG):
	g++ -o $(TEST_LOG) $(INCLUDE_PATH) $(CFLAGS) $(TEST_LOG).cpp -L../lib -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

$(TUTORIAL_FMT_STREAM):
	g++ -o $(TUTORIAL_FMT_STREAM) $(INCLUDE_PATH) $(CFLAGS) $(TUTORIAL_FMT_STREAM).cpp -L../lib -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread -lrt

$(TEST_TRIVIAL_LOGGING):
	g++ -o $(TEST_TRIVIAL_LOGGING) $(INCLUDE_PATH) $(CFLAGS) $(TEST_TRIVIAL_LOGGING).cpp -L../lib -lboost_log -lboost_system -lboost_thread -lpthread -lrt

test_lexicalcase:
	g++ test_lexicalcase.cpp -I ../third/linux/include/boost/

clean:
	rm -f $(TARGETS)
