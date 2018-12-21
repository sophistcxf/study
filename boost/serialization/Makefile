CXX = g++

CFLAGS= -g 
INCLUDE_PATH=-I ../../include
LIB_PATH=-L ../../lib

test_archive:
	g++ -o test_archive $(INCLUDE_PATH) $(CFLAGS) test_archive.cpp $(LIB_PATH) -lpthread -lrt -lboost_serialization ${LIB_PATH}/libboost_exception.a

clean:
	rm -f test_archive 
