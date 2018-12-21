CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib


TEST_BM=test_boyer_moore

test_gzip:
	g++ -o test_gzip $(INCLUDE_PATH) $(CFLAGS) test_gzip.cpp $(LIB_PATH) -lboost_iostreams -lpthread -lrt -lz

clean:
	rm -f $(TARGETS)
