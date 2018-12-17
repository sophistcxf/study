CXX = g++

CFLAGS=-std=c++98 -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

TARGETS=

all: $(TARGETS)

test_parse_xml:
	g++ -o test_parse_xml $(INCLUDE_PATH) $(CFLAGS) test_parse_xml.cpp $(LIB_PATH) -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread -lrt

clean:
	rm -f $(TARGETS)
