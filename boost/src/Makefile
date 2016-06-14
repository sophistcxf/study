CXX = g++

CFLAGS=-std=c++98 -g 
INCLUDE_PATH=-I ../include

TEST_TOKENIZER=test_tokenizer
TEST_LOG=test_log

TARGETS=$(TEST_TOKENIZER) $(TEST_LOG)

all: $(TARGETS)

$(TEST_TOKENIZER):
	g++ -o $(TEST_TOKENIZER) $(INCLUDE_PATH) $(CFLAGS) $(TEST_TOKENIZER).cpp

$(TEST_LOG):
	g++ -o $(TEST_LOG) $(INCLUDE_PATH) $(CFLAGS) $(TEST_LOG).cpp -L../lib -lboost_log -lboost_system -lboost_thread -lpthread -lrt

clean:
	rm -f $(TARGETS)
